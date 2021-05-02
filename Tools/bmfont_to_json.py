#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys


def process_array(data):
    result = []
    t = 0
    ts = ''
    for d in data:
        t += d.count('\"')
        ts += ' ' + d
        if t % 2 == 0:
            ts = ts.strip()
            result.append(ts)
            ts = ''
    return result


def main():
    if len(sys.argv) < 2:
        print('Usage: python bmfont_to_json.py <fnt_file> <json_file>')
        return 
    fnt_file = sys.argv[1]
    json_file = sys.argv[2]

    input_file = open(fnt_file, 'r')
    bmfont_strs = input_file.readlines()
    input_file.close()

    page_list = '['
    char_list = '['
    json_str = '{'
    for bmfont_str in bmfont_strs:
        data = bmfont_str.split()
        data = process_array(data)
        if data and data[0] == 'chars':
            continue
        cur_str = '\n'
        if data and data[0] != 'page' and data[0] != 'char':
            cur_str += '"%s":' % (data[0],)
        cur_str += '{'
        for d in data[1:]:
            i = d.find('=')
            if not i == -1:
                key, value = d[0:i], d[i+1:]
                if key == 'pages':
                    continue
                if value.find(',') != -1:
                    value = '[' + value + ']'
                cur_str += '"%s":%s, ' % (key, value)
        if cur_str[-2:] == ', ':
            cur_str = cur_str[:-2]
        cur_str += '},'
        if data:
            if data[0] == 'page':
                page_list += cur_str
            elif data[0] == 'char':
                char_list += cur_str
            else:
                json_str += cur_str
    if page_list[-1] == ',':
        page_list = page_list[:-1]
    if char_list[-1] == ',':
        char_list = char_list[:-1]
    page_list += '\n]'
    char_list += '\n]'
    json_str += '\n"pages":' + page_list + ','
    json_str += '\n"chars":' + char_list
    json_str += '\n}'

    output_file = open(json_file, 'w')
    output_file.write(json_str)
    output_file.close()


if __name__ == '__main__':
    main()
