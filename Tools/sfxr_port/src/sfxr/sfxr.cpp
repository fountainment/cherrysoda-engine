/*
   Copyright (c) 2007 Tomas Pettersson <drpetter@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

*/

// #include "sdlkit.h"
#include "tools.h"

#include <CherrySoda/CherrySoda.h>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif // __EMSCRIPTEN__

using namespace cherrysoda;

SDL_AudioSpec des;
bool firstPlay = true;

#define rnd(n) (std::rand()%(n+1))

#define PI 3.14159265f

inline float frnd(float range) { return (float)rnd(10000)/10000*range; }

int wave_type;

float p_base_freq;
float p_freq_limit;
float p_freq_ramp;
float p_freq_dramp;
float p_duty;
float p_duty_ramp;

float p_vib_strength;
float p_vib_speed;
float p_vib_delay;

float p_env_attack;
float p_env_sustain;
float p_env_decay;
float p_env_punch;

bool filter_on;
float p_lpf_resonance;
float p_lpf_freq;
float p_lpf_ramp;
float p_hpf_freq;
float p_hpf_ramp;

float p_pha_offset;
float p_pha_ramp;

float p_repeat_speed;

float p_arp_speed;
float p_arp_mod;

float master_vol=0.05f;

float sound_vol=0.5f;


bool playing_sample=false;
int phase;
double fperiod;
double fmaxperiod;
double fslide;
double fdslide;
int period;
float square_duty;
float square_slide;
int env_stage;
int env_time;
int env_length[3];
float env_vol;
float fphase;
float fdphase;
int iphase;
float phaser_buffer[1024];
int ipp;
float noise_buffer[32];
float fltp;
float fltdp;
float fltw;
float fltw_d;
float fltdmp;
float fltphp;
float flthp;
float flthp_d;
float vib_phase;
float vib_speed;
float vib_amp;
int rep_time;
int rep_limit;
int arp_time;
int arp_limit;
double arp_mod;

int wav_bits=16;
int wav_freq=44100;

int file_sampleswritten;
float filesample=0.0f;
int fileacc=0;

float* lastChanged = NULL;  // For undo
float undoValue = 0.0f;

void SetUndo(float* valueptr, float oldValue)
{
	lastChanged = valueptr;
	undoValue = oldValue;
}

void Undo()
{
	if(lastChanged != NULL)
	{
		*lastChanged = undoValue;
		lastChanged = NULL;
		undoValue = 0.0f;
	}
}

void ResetParams()
{
	lastChanged = NULL;
	wave_type=0;

	p_base_freq=0.3f;
	p_freq_limit=0.0f;
	p_freq_ramp=0.0f;
	p_freq_dramp=0.0f;
	p_duty=0.0f;
	p_duty_ramp=0.0f;

	p_vib_strength=0.0f;
	p_vib_speed=0.0f;
	p_vib_delay=0.0f;

	p_env_attack=0.0f;
	p_env_sustain=0.3f;
	p_env_decay=0.4f;
	p_env_punch=0.0f;

	filter_on=false;
	p_lpf_resonance=0.0f;
	p_lpf_freq=1.0f;
	p_lpf_ramp=0.0f;
	p_hpf_freq=0.0f;
	p_hpf_ramp=0.0f;
	
	p_pha_offset=0.0f;
	p_pha_ramp=0.0f;

	p_repeat_speed=0.0f;

	p_arp_speed=0.0f;
	p_arp_mod=0.0f;
}

bool LoadSettings(char* filename)
{
	lastChanged = NULL;
	FILE* file=fopen(filename, "rb");
	if(!file)
		return false;
	
	size_t n;
	(void)n;
	int version=0;
	n = fread(&version, 1, sizeof(int), file);
	if(version!=100 && version!=101 && version!=102)
		return false;

	n = fread(&wave_type, 1, sizeof(int), file);

	sound_vol=0.5f;
	if(version==102)
		n = fread(&sound_vol, 1, sizeof(float), file);

	n = fread(&p_base_freq, 1, sizeof(float), file);
	n = fread(&p_freq_limit, 1, sizeof(float), file);
	n = fread(&p_freq_ramp, 1, sizeof(float), file);
	if(version>=101)
		n = fread(&p_freq_dramp, 1, sizeof(float), file);
	n = fread(&p_duty, 1, sizeof(float), file);
	n = fread(&p_duty_ramp, 1, sizeof(float), file);

	n = fread(&p_vib_strength, 1, sizeof(float), file);
	n = fread(&p_vib_speed, 1, sizeof(float), file);
	n = fread(&p_vib_delay, 1, sizeof(float), file);

	n = fread(&p_env_attack, 1, sizeof(float), file);
	n = fread(&p_env_sustain, 1, sizeof(float), file);
	n = fread(&p_env_decay, 1, sizeof(float), file);
	n = fread(&p_env_punch, 1, sizeof(float), file);

	n = fread(&filter_on, 1, sizeof(bool), file);
	n = fread(&p_lpf_resonance, 1, sizeof(float), file);
	n = fread(&p_lpf_freq, 1, sizeof(float), file);
	n = fread(&p_lpf_ramp, 1, sizeof(float), file);
	n = fread(&p_hpf_freq, 1, sizeof(float), file);
	n = fread(&p_hpf_ramp, 1, sizeof(float), file);
	
	n = fread(&p_pha_offset, 1, sizeof(float), file);
	n = fread(&p_pha_ramp, 1, sizeof(float), file);

	n = fread(&p_repeat_speed, 1, sizeof(float), file);

	if(version>=101)
	{
		n = fread(&p_arp_speed, 1, sizeof(float), file);
		n = fread(&p_arp_mod, 1, sizeof(float), file);
	}

	fclose(file);
	return true;
}

bool SaveSettings(char* filename)
{
	FILE* file=fopen(filename, "wb");
	if(!file)
		return false;

	int version=102;
	fwrite(&version, 1, sizeof(int), file);

	fwrite(&wave_type, 1, sizeof(int), file);

	fwrite(&sound_vol, 1, sizeof(float), file);

	fwrite(&p_base_freq, 1, sizeof(float), file);
	fwrite(&p_freq_limit, 1, sizeof(float), file);
	fwrite(&p_freq_ramp, 1, sizeof(float), file);
	fwrite(&p_freq_dramp, 1, sizeof(float), file);
	fwrite(&p_duty, 1, sizeof(float), file);
	fwrite(&p_duty_ramp, 1, sizeof(float), file);

	fwrite(&p_vib_strength, 1, sizeof(float), file);
	fwrite(&p_vib_speed, 1, sizeof(float), file);
	fwrite(&p_vib_delay, 1, sizeof(float), file);

	fwrite(&p_env_attack, 1, sizeof(float), file);
	fwrite(&p_env_sustain, 1, sizeof(float), file);
	fwrite(&p_env_decay, 1, sizeof(float), file);
	fwrite(&p_env_punch, 1, sizeof(float), file);

	fwrite(&filter_on, 1, sizeof(bool), file);
	fwrite(&p_lpf_resonance, 1, sizeof(float), file);
	fwrite(&p_lpf_freq, 1, sizeof(float), file);
	fwrite(&p_lpf_ramp, 1, sizeof(float), file);
	fwrite(&p_hpf_freq, 1, sizeof(float), file);
	fwrite(&p_hpf_ramp, 1, sizeof(float), file);
	
	fwrite(&p_pha_offset, 1, sizeof(float), file);
	fwrite(&p_pha_ramp, 1, sizeof(float), file);

	fwrite(&p_repeat_speed, 1, sizeof(float), file);

	fwrite(&p_arp_speed, 1, sizeof(float), file);
	fwrite(&p_arp_mod, 1, sizeof(float), file);

	fclose(file);
	return true;
}

void ResetSample(bool restart)
{
	if(!restart)
		phase=0;
	fperiod=100.0/(p_base_freq*p_base_freq+0.001);
	period=(int)fperiod;
	fmaxperiod=100.0/(p_freq_limit*p_freq_limit+0.001);
	fslide=1.0-pow((double)p_freq_ramp, 3.0)*0.01;
	fdslide=-pow((double)p_freq_dramp, 3.0)*0.000001;
	square_duty=0.5f-p_duty*0.5f;
	square_slide=-p_duty_ramp*0.00005f;
	if(p_arp_mod>=0.0f)
		arp_mod=1.0-pow((double)p_arp_mod, 2.0)*0.9;
	else
		arp_mod=1.0+pow((double)p_arp_mod, 2.0)*10.0;
	arp_time=0;
	arp_limit=(int)(pow(1.0f-p_arp_speed, 2.0f)*20000+32);
	if(p_arp_speed==1.0f)
		arp_limit=0;
	if(!restart)
	{
		// reset filter
		fltp=0.0f;
		fltdp=0.0f;
		fltw=pow(p_lpf_freq, 3.0f)*0.1f;
		fltw_d=1.0f+p_lpf_ramp*0.0001f;
		fltdmp=5.0f/(1.0f+pow(p_lpf_resonance, 2.0f)*20.0f)*(0.01f+fltw);
		if(fltdmp>0.8f) fltdmp=0.8f;
		fltphp=0.0f;
		flthp=pow(p_hpf_freq, 2.0f)*0.1f;
		flthp_d=1.0+p_hpf_ramp*0.0003f;
		// reset vibrato
		vib_phase=0.0f;
		vib_speed=pow(p_vib_speed, 2.0f)*0.01f;
		vib_amp=p_vib_strength*0.5f;
		// reset envelope
		env_vol=0.0f;
		env_stage=0;
		env_time=0;
		env_length[0]=(int)(p_env_attack*p_env_attack*100000.0f);
		env_length[1]=(int)(p_env_sustain*p_env_sustain*100000.0f);
		env_length[2]=(int)(p_env_decay*p_env_decay*100000.0f);

		fphase=pow(p_pha_offset, 2.0f)*1020.0f;
		if(p_pha_offset<0.0f) fphase=-fphase;
		fdphase=pow(p_pha_ramp, 2.0f)*1.0f;
		if(p_pha_ramp<0.0f) fdphase=-fdphase;
		iphase=abs((int)fphase);
		ipp=0;
		for(int i=0;i<1024;i++)
			phaser_buffer[i]=0.0f;

		for(int i=0;i<32;i++)
			noise_buffer[i]=frnd(2.0f)-1.0f;

		rep_time=0;
		rep_limit=(int)(pow(1.0f-p_repeat_speed, 2.0f)*20000+32);
		if(p_repeat_speed==0.0f)
			rep_limit=0;
	}
}

void PlaySample()
{
	if (firstPlay) {
		SDL_OpenAudio(&des, NULL);
		SDL_PauseAudio(0);
		firstPlay = false;
	}
	ResetSample(false);
	playing_sample=true;
}

void SynthSample(int length, float* buffer, FILE* file)
{
	for(int i=0;i<length;i++)
	{
		if(!playing_sample)
			break;

		rep_time++;
		if(rep_limit!=0 && rep_time>=rep_limit)
		{
			rep_time=0;
			ResetSample(true);
		}

		// frequency envelopes/arpeggios
		arp_time++;
		if(arp_limit!=0 && arp_time>=arp_limit)
		{
			arp_limit=0;
			fperiod*=arp_mod;
		}
		fslide+=fdslide;
		fperiod*=fslide;
		if(fperiod>fmaxperiod)
		{
			fperiod=fmaxperiod;
			if(p_freq_limit>0.0f)
				playing_sample=false;
		}
		float rfperiod=fperiod;
		if(vib_amp>0.0f)
		{
			vib_phase+=vib_speed;
			rfperiod=fperiod*(1.0+sin(vib_phase)*vib_amp);
		}
		period=(int)rfperiod;
		if(period<8) period=8;
		square_duty+=square_slide;
		if(square_duty<0.0f) square_duty=0.0f;
		if(square_duty>0.5f) square_duty=0.5f;		
		// volume envelope
		env_time++;
		if(env_time>env_length[env_stage])
		{
			env_time=0;
			env_stage++;
			if(env_stage==3)
				playing_sample=false;
		}
		if(env_stage==0)
			env_vol=(float)env_time/env_length[0];
		if(env_stage==1)
			env_vol=1.0f+pow(1.0f-(float)env_time/env_length[1], 1.0f)*2.0f*p_env_punch;
		if(env_stage==2)
			env_vol=1.0f-(float)env_time/env_length[2];

		// phaser step
		fphase+=fdphase;
		iphase=abs((int)fphase);
		if(iphase>1023) iphase=1023;

		if(flthp_d!=0.0f)
		{
			flthp*=flthp_d;
			if(flthp<0.00001f) flthp=0.00001f;
			if(flthp>0.1f) flthp=0.1f;
		}

		float ssample=0.0f;
		for(int si=0;si<8;si++) // 8x supersampling
		{
			float sample=0.0f;
			phase++;
			if(phase>=period)
			{
//				phase=0;
				phase%=period;
				if(wave_type==3)
					for(int i=0;i<32;i++)
						noise_buffer[i]=frnd(2.0f)-1.0f;
			}
			// base waveform
			float fp=(float)phase/period;
			switch(wave_type)
			{
			case 0: // square
				if(fp<square_duty)
					sample=0.5f;
				else
					sample=-0.5f;
				break;
			case 1: // sawtooth
				sample=1.0f-fp*2;
				break;
			case 2: // sine
				sample=(float)sin(fp*2*PI);
				break;
			case 3: // noise
				sample=noise_buffer[phase*32/period];
				break;
			}
			// lp filter
			float pp=fltp;
			fltw*=fltw_d;
			if(fltw<0.0f) fltw=0.0f;
			if(fltw>0.1f) fltw=0.1f;
			if(p_lpf_freq!=1.0f)
			{
				fltdp+=(sample-fltp)*fltw;
				fltdp-=fltdp*fltdmp;
			}
			else
			{
				fltp=sample;
				fltdp=0.0f;
			}
			fltp+=fltdp;
			// hp filter
			fltphp+=fltp-pp;
			fltphp-=fltphp*flthp;
			sample=fltphp;
			// phaser
			phaser_buffer[ipp&1023]=sample;
			sample+=phaser_buffer[(ipp-iphase+1024)&1023];
			ipp=(ipp+1)&1023;
			// final accumulation and envelope application
			ssample+=sample*env_vol;
		}
		ssample=ssample/8*master_vol;

		ssample*=2.0f*sound_vol;

		if(buffer!=NULL)
		{
			if(ssample>1.0f) ssample=1.0f;
			if(ssample<-1.0f) ssample=-1.0f;
			*buffer++=ssample;
		}
		if(file!=NULL)
		{
			// quantize depending on format
			// accumulate/count to accomodate variable sample rate?
			ssample*=4.0f; // arbitrary gain to get reasonable output volume...
			if(ssample>1.0f) ssample=1.0f;
			if(ssample<-1.0f) ssample=-1.0f;
			filesample+=ssample;
			fileacc++;
			if(wav_freq==44100 || fileacc==2)
			{
				filesample/=fileacc;
				fileacc=0;
				if(wav_bits==16)
				{
					short isample=(short)(filesample*32000);
					fwrite(&isample, 1, 2, file);
				}
				else
				{
					unsigned char isample=(unsigned char)(filesample*127+128);
					fwrite(&isample, 1, 1, file);
				}
				filesample=0.0f;
			}
			file_sampleswritten++;
		}
	}
}

bool mute_stream;

//lets use SDL instead
static void SDLAudioCallback(void *userdata, Uint8 *stream, int len)
{
	static float s_fbuf[2048];
	if (playing_sample && !mute_stream)
	{
		unsigned int l = len/2;
		float* fbuf = s_fbuf;
		memset(fbuf, 0, sizeof(*fbuf));
		SynthSample(l, fbuf, NULL);
		while (l--)
		{
			float f = fbuf[l];
			if (f < -1.0) f = -1.0;
			if (f > 1.0) f = 1.0;
			((Sint16*)stream)[l] = (Sint16)(f * 32767);
		}
	}
	else memset(stream, 0, len);
}

bool ExportWAV(const char* filename)
{
	FILE* foutput=fopen(filename, "wb");
	if(!foutput)
		return false;
	// write wav header
	unsigned int dword=0;
	unsigned short word=0;
	fwrite("RIFF", 4, 1, foutput); // "RIFF"
	dword=0;
	fwrite(&dword, 1, 4, foutput); // remaining file size
	fwrite("WAVE", 4, 1, foutput); // "WAVE"

	fwrite("fmt ", 4, 1, foutput); // "fmt "
	dword=16;
	fwrite(&dword, 1, 4, foutput); // chunk size
	word=1;
	fwrite(&word, 1, 2, foutput); // compression code
	word=1;
	fwrite(&word, 1, 2, foutput); // channels
	dword=wav_freq;
	fwrite(&dword, 1, 4, foutput); // sample rate
	dword=wav_freq*wav_bits/8;
	fwrite(&dword, 1, 4, foutput); // bytes/sec
	word=wav_bits/8;
	fwrite(&word, 1, 2, foutput); // block align
	word=wav_bits;
	fwrite(&word, 1, 2, foutput); // bits per sample

	fwrite("data", 4, 1, foutput); // "data"
	dword=0;
	int foutstream_datasize=ftell(foutput);
	fwrite(&dword, 1, 4, foutput); // chunk size

	// write sample data
	mute_stream=true;
	file_sampleswritten=0;
	filesample=0.0f;
	fileacc=0;
	PlaySample();
	while(playing_sample)
		SynthSample(256, NULL, foutput);
	mute_stream=false;

	// seek back to header and write size info
	fseek(foutput, 4, SEEK_SET);
	dword=0;
	dword=foutstream_datasize-4+file_sampleswritten*wav_bits/8;
	fwrite(&dword, 1, 4, foutput); // remaining file size
	fseek(foutput, foutstream_datasize, SEEK_SET);
	dword=file_sampleswritten*wav_bits/8;
	fwrite(&dword, 1, 4, foutput); // chunk size (data)
	fclose(foutput);

#ifdef __EMSCRIPTEN__
	String cmd = CHERRYSODA_FORMAT("saveFileFromMemoryFSToDisk('/%s','%s')", filename, filename);
	emscripten_run_script(cmd.c_str());
#endif // __EMSCRIPTEN__
	
	return true;
}

#include "tools.h"

bool firstframe=true;
int refresh_counter=0;

void Randomize()
{
	wave_type = rnd(3);
	p_base_freq=pow(frnd(2.0f)-1.0f, 2.0f);
	if(rnd(1))
		p_base_freq=pow(frnd(2.0f)-1.0f, 3.0f)+0.5f;
	p_freq_limit=0.0f;
	p_freq_ramp=pow(frnd(2.0f)-1.0f, 5.0f);
	if(p_base_freq>0.7f && p_freq_ramp>0.2f)
		p_freq_ramp=-p_freq_ramp;
	if(p_base_freq<0.2f && p_freq_ramp<-0.05f)
		p_freq_ramp=-p_freq_ramp;
	p_freq_dramp=pow(frnd(2.0f)-1.0f, 3.0f);
	p_duty=frnd(2.0f)-1.0f;
	p_duty_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
	p_vib_strength=pow(frnd(2.0f)-1.0f, 3.0f);
	p_vib_speed=frnd(2.0f)-1.0f;
	p_vib_delay=frnd(2.0f)-1.0f;
	p_env_attack=pow(frnd(2.0f)-1.0f, 3.0f);
	p_env_sustain=pow(frnd(2.0f)-1.0f, 2.0f);
	p_env_decay=frnd(2.0f)-1.0f;
	p_env_punch=pow(frnd(0.8f), 2.0f);
	if(p_env_attack+p_env_sustain+p_env_decay<0.2f)
	{
		p_env_sustain+=0.2f+frnd(0.3f);
		p_env_decay+=0.2f+frnd(0.3f);
	}
	p_lpf_resonance=frnd(2.0f)-1.0f;
	p_lpf_freq=1.0f-pow(frnd(1.0f), 3.0f);
	p_lpf_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
	if(p_lpf_freq<0.1f && p_lpf_ramp<-0.05f)
		p_lpf_ramp=-p_lpf_ramp;
	p_hpf_freq=pow(frnd(1.0f), 5.0f);
	p_hpf_ramp=pow(frnd(2.0f)-1.0f, 5.0f);
	p_pha_offset=pow(frnd(2.0f)-1.0f, 3.0f);
	p_pha_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
	p_repeat_speed=frnd(2.0f)-1.0f;
	p_arp_speed=frnd(2.0f)-1.0f;
	p_arp_mod=frnd(2.0f)-1.0f;
}

void Mutate()
{
	if(rnd(1)) p_base_freq+=frnd(0.1f)-0.05f;
//		if(rnd(1)) p_freq_limit+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_freq_ramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_freq_dramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_duty+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_duty_ramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_vib_strength+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_vib_speed+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_vib_delay+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_env_attack+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_env_sustain+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_env_decay+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_env_punch+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_lpf_resonance+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_lpf_freq+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_lpf_ramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_hpf_freq+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_hpf_ramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_pha_offset+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_pha_ramp+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_repeat_speed+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_arp_speed+=frnd(0.1f)-0.05f;
	if(rnd(1)) p_arp_mod+=frnd(0.1f)-0.05f;
}

void DrawScreen()
{
	static int lang_i = 1;
	#define LANGS(A,B) (lang_i?(B):(A))
	bool do_play = false;
	ImGui::Begin("sfxr", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
	{
		ImGui::SetWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetWindowSize(ImVec2(Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight()));
		
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("Generator", ImVec2(200.f, 0), true);
			{
				ImGui::Text(LANGS(u8"生成器", "Generator"));
				// PickUp/Coin
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"拾取/金币", "PickUp/Coin"), ImVec2(170.f, 0.f))) {
					ResetParams();
					p_base_freq = 0.4f + frnd(0.5f);
					p_env_attack = 0.0f;
					p_env_sustain = frnd(0.1f);
					p_env_decay = 0.1f + frnd(0.4f);
					p_env_punch = 0.3f + frnd(0.3f);
					if (rnd(1))
					{
						p_arp_speed = 0.5f + frnd(0.2f);
						p_arp_mod = 0.2f + frnd(0.4f);
					}
					PlaySample();
				}
				// Laser/Shoot
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"激光/射击", "Laser/Shoot"), ImVec2(170.f, 0.f))) {
					ResetParams();
					wave_type = rnd(2);
					if (wave_type == 2 && rnd(1))
						wave_type = rnd(1);
					p_base_freq = 0.5f + frnd(0.5f);
					p_freq_limit = p_base_freq - 0.2f - frnd(0.6f);
					if (p_freq_limit < 0.2f) p_freq_limit = 0.2f;
					p_freq_ramp = -0.15f - frnd(0.2f);
					if (rnd(2) == 0)
					{
						p_base_freq = 0.3f + frnd(0.6f);
						p_freq_limit = frnd(0.1f);
						p_freq_ramp = -0.35f - frnd(0.3f);
					}
					if (rnd(1))
					{
						p_duty = frnd(0.5f);
						p_duty_ramp = frnd(0.2f);
					}
					else
					{
						p_duty = 0.4f + frnd(0.5f);
						p_duty_ramp = -frnd(0.7f);
					}
					p_env_attack = 0.0f;
					p_env_sustain = 0.1f + frnd(0.2f);
					p_env_decay = frnd(0.4f);
					if (rnd(1))
						p_env_punch = frnd(0.3f);
					if (rnd(2) == 0)
					{
						p_pha_offset = frnd(0.2f);
						p_pha_ramp = -frnd(0.2f);
					}
					if (rnd(1))
						p_hpf_freq = frnd(0.3f);
					PlaySample();
				}
				// Explosion
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"爆炸", "Explosion"), ImVec2(170.f, 0.f))) {
					ResetParams();
					wave_type = 3;
					if (rnd(1))
					{
						p_base_freq = 0.1f + frnd(0.4f);
						p_freq_ramp = -0.1f + frnd(0.4f);
					}
					else
					{
						p_base_freq = 0.2f + frnd(0.7f);
						p_freq_ramp = -0.2f - frnd(0.2f);
					}
					p_base_freq *= p_base_freq;
					if (rnd(4) == 0)
						p_freq_ramp = 0.0f;
					if (rnd(2) == 0)
						p_repeat_speed = 0.3f + frnd(0.5f);
					p_env_attack = 0.0f;
					p_env_sustain = 0.1f + frnd(0.3f);
					p_env_decay = frnd(0.5f);
					if (rnd(1) == 0)
					{
						p_pha_offset = -0.3f + frnd(0.9f);
						p_pha_ramp = -frnd(0.3f);
					}
					p_env_punch = 0.2f + frnd(0.6f);
					if (rnd(1))
					{
						p_vib_strength = frnd(0.7f);
						p_vib_speed = frnd(0.6f);
					}
					if (rnd(2) == 0)
					{
						p_arp_speed = 0.6f + frnd(0.3f);
						p_arp_mod = 0.8f - frnd(1.6f);
					}
					PlaySample();
				}
				// PowerUp
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"升级", "PowerUp"), ImVec2(170.f, 0.f))) {
					ResetParams();
					if (rnd(1))
						wave_type = 1;
					else
						p_duty = frnd(0.6f);
					if (rnd(1))
					{
						p_base_freq = 0.2f + frnd(0.3f);
						p_freq_ramp = 0.1f + frnd(0.4f);
						p_repeat_speed = 0.4f + frnd(0.4f);
					}
					else
					{
						p_base_freq = 0.2f + frnd(0.3f);
						p_freq_ramp = 0.05f + frnd(0.2f);
						if (rnd(1))
						{
							p_vib_strength = frnd(0.7f);
							p_vib_speed = frnd(0.6f);
						}
					}
					p_env_attack = 0.0f;
					p_env_sustain = frnd(0.4f);
					p_env_decay = 0.1f + frnd(0.4f);
					PlaySample();
				}
				// Hit/Hurt
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"击中/受伤", "Hit/Hurt"), ImVec2(170.f, 0.f))) {
					ResetParams();
					wave_type = rnd(2);
					if (wave_type == 2)
						wave_type = 3;
					if (wave_type == 0)
						p_duty = frnd(0.6f);
					p_base_freq = 0.2f + frnd(0.6f);
					p_freq_ramp = -0.3f - frnd(0.4f);
					p_env_attack = 0.0f;
					p_env_sustain = frnd(0.1f);
					p_env_decay = 0.1f + frnd(0.2f);
					if (rnd(1))
						p_hpf_freq = frnd(0.3f);
					PlaySample();
				}
				// Jump
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"跳跃", "Jump"), ImVec2(170.f, 0.f))) {
					ResetParams();
					wave_type = 0;
					p_duty = frnd(0.6f);
					p_base_freq = 0.3f + frnd(0.3f);
					p_freq_ramp = 0.1f + frnd(0.2f);
					p_env_attack = 0.0f;
					p_env_sustain = 0.1f + frnd(0.3f);
					p_env_decay = 0.1f + frnd(0.2f);
					if (rnd(1))
						p_hpf_freq = frnd(0.3f);
					if (rnd(1))
						p_lpf_freq = 1.0f - frnd(0.6f);
					PlaySample();
				}
				// Blip/Select
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"哔哔声/选择", "Blip/Select"), ImVec2(170.f, 0.f))) {
					ResetParams();
					wave_type = rnd(1);
					if (wave_type == 0)
						p_duty = frnd(0.6f);
					p_base_freq = 0.2f + frnd(0.4f);
					p_env_attack = 0.0f;
					p_env_sustain = 0.1f + frnd(0.1f);
					p_env_decay = frnd(0.2f);
					p_hpf_freq = 0.1f;
					PlaySample();
				}

				ImGui::Spacing(); ImGui::NewLine();

				// Mutate
				if (ImGui::Button(LANGS(u8"变异", "Mutate"), ImVec2(140.f, 0.f))) {
					Mutate();
					do_play = true;
				}
				// Randomize
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"随机", "Randomize"), ImVec2(140.f, 0.f))) {
					Randomize();
					do_play = true;
				}

				ImGui::Spacing(); ImGui::NewLine();

				// Volume
				ImGui::Text(LANGS(u8"音量", "Volume"));
				if (ImGui::SliderFloat("", &sound_vol, 0.f, 1.f)) {
					PlaySample();
				}
				// Play Sound
				ImGui::Spacing();
				if (ImGui::Button(LANGS(u8"播放声音", "Play Sound"), ImVec2(170.f, 0.f))) {
					PlaySample();
				}

				ImGui::Spacing(); ImGui::NewLine();

				// Export WAV
				if (ImGui::Button(LANGS(u8"导出WAV", "Export WAV"), ImVec2(170.f, 0.f))) {
					ExportWAV("export.wav");
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndGroup(); ImGui::SameLine();

		ImGui::BeginGroup();
		{
			ImGui::BeginChild("Manual Settings", ImVec2(0, 0), true);
			{
				ImGui::Text(LANGS(u8"手动设置", "Manual Settings")); ImGui::SameLine(ImGui::GetWindowWidth() - 40.f);
				const char* lang[] = { "En", u8"中" };
				if (ImGui::Button(lang[lang_i])) {
					if (lang_i == 0) {
						lang_i = 1;
					}
					else {
						lang_i = 0;
					}
				}
				ImGui::Spacing();
				float rbWidth = 180.f;
				if (ImGui::RadioButton(LANGS(u8"方波", "Squarewave"), &wave_type, 0)) { do_play = true; } ImGui::SameLine(rbWidth);
				if (ImGui::RadioButton(LANGS(u8"锯齿波", "Sawtooth"), &wave_type, 1)) { do_play = true; } ImGui::SameLine(rbWidth * 2.f);
				if (ImGui::RadioButton(LANGS(u8"正弦波", "Sinewave"), &wave_type, 2)) { do_play = true; } ImGui::SameLine(rbWidth * 3.f);
				if (ImGui::RadioButton(LANGS(u8"噪声", "Noise"), &wave_type, 3)) { do_play = true; }

				ImGui::Spacing();
				ImGui::BeginChild("Parameters", ImVec2(0, 0), true);
				{
					ImGui::SliderFloat(LANGS(u8"起音时长", "Attack Time"), &p_env_attack, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"延音时长", "Sustain Time"), &p_env_sustain, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"延音打击", "Sustain Punch"), &p_env_punch, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"衰减时长", "Decay Time"), &p_env_decay, 0.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"初始频率", "Start Frequency"), &p_base_freq, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"最低频率", "Min Frequency"), &p_freq_limit, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"滑音速度", "Slide"), &p_freq_ramp, -1.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"滑音加速度", "Delta Slide"), &p_freq_dramp, -1.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"颤音深度", "Vibrato Depth"), &p_vib_strength, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"颤音速率", "Vibrato Speed"), &p_vib_speed, 0.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"瞬时音高变化", "Change Amount"), &p_arp_mod, -1.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"变化速度", "Change Speed"), &p_arp_speed, 0.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"方波占空比", "Square Duty"), &p_duty, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"占空比扫描", "Duty Sweep"), &p_duty_ramp, -1.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"重复速率", "Repeat Speed"), &p_repeat_speed, 0.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"移相", "Phaser Offset"), &p_pha_offset, -1.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"移相扫描", "Phaser Sweep"), &p_pha_ramp, -1.f, 1.f);

					ImGui::Separator();
					ImGui::SliderFloat(LANGS(u8"低通滤波截止频率", "LP Filer Cutoff"), &p_lpf_freq, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"低通滤波截止频率扫描", "LP Filer Cutoff Sweep"), &p_lpf_ramp, -1.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"低通滤波共鸣", "LP Filer Resonance"), &p_lpf_resonance, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"高通滤波截止频率", "HP Filer Cutoff"), &p_hpf_freq, 0.f, 1.f);
					ImGui::SliderFloat(LANGS(u8"高通滤波截止频率扫描", "HP Filer Cutoff Sweep"), &p_hpf_ramp, -1.f, 1.f);
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
		ImGui::EndGroup();
	}
	ImGui::End();

	// Port Begin
	// if(Button(490, 290, false, "LOAD SOUND", 14))
	// {
	// 	char filename[256];
	// 	if(FileSelectorLoad(filename, 1))
	// 	{
	// 		ResetParams();
	// 		LoadSettings(filename);
	// 		PlaySample();
	// 	}
	// }
	// if(Button(490, 320, false, "SAVE SOUND", 15))
	// {
	// 	char filename[256];
	// 	if(FileSelectorSave(filename, 1))
	// 		SaveSettings(filename);
	// }
	// Port End

	if(do_play)
		PlaySample();
}


bool SfxrUpdate()
{
	static bool keydown = false;
	if(MInput::Keyboard()->Pressed(Keys::Space) || MInput::Keyboard()->Pressed(Keys::Enter)) {
	 	if (!keydown) {
	 		PlaySample();
	 		keydown = true;
	 	}
	}
	else if (MInput::Keyboard()->Pressed(Keys::R)) {
		if (!keydown) {
			Randomize();
			PlaySample();
			keydown = true;
		}
	}
	else if(MInput::Keyboard()->Pressed(Keys::Z)) {
	 	Undo();
	 	keydown = true;
	}
	else
	 	keydown = false;

	DrawScreen();

	return true;
}


void SfxrInit()
{
	srand(time(NULL));

	ImGuiIO& io = ImGui::GetIO();
	// io.FontGlobalScale = 2.0f;

	// ImVector<ImWchar> gRanges;
	// ImFontGlyphRangesBuilder gBuilder;
	// gBuilder.AddText(u8"");
	// gBuilder.BuildRanges(&gRanges);

	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("./assets/wqy.ttf", 26.f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	GUI::BuildFontTexture();

	ResetParams();

	des.freq = 44100;
	des.format = AUDIO_S16SYS;
	des.channels = 1;
	des.samples = 512;
	des.callback = SDLAudioCallback;
	des.userdata = NULL;
}
