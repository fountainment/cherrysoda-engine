from datetime import datetime
import curses
import time

def main():
    stdscr = curses.initscr()
    curses.noecho()

    while True:
        stdscr.clear()
        remaining_time = datetime(2020, 3, 1) - datetime.now()
        stdscr.addstr(str(remaining_time) + '\n')
        stdscr.addstr(str(remaining_time.total_seconds()))
        stdscr.refresh()
        time.sleep(1)

if __name__ == '__main__':
    main()
