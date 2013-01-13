/*
 * GeekOS C code entry point
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2004, Iulian Neamtiu <neamtiu@cs.umd.edu>
 * $Revision: 1.51 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */


#include <geekos/bootinfo.h>
#include <geekos/string.h>
#include <geekos/screen.h>
#include <geekos/mem.h>
#include <geekos/crc32.h>
#include <geekos/tss.h>
#include <geekos/int.h>
#include <geekos/kthread.h>
#include <geekos/trap.h>
#include <geekos/timer.h>
#include <geekos/keyboard.h>




/*
 * Kernel C code entry point.
 * Initializes kernel subsystems, mounts filesystems,
 * and spawns init process.
 */

void KeyFunc(ulong_t );

void Main(struct Boot_Info* bootInfo)
{
    Init_BSS();
    Init_Screen();
    Init_Mem(bootInfo);
    Init_CRC32();
    Init_TSS();
    Init_Interrupts();
    Init_Scheduler();
    Init_Traps();
    Init_Timer();
    Init_Keyboard();


    Set_Current_Attr(ATTRIB(BLACK, GREEN|BRIGHT));
    Set_Current_Attr(ATTRIB(BLACK, GRAY));

    Start_Kernel_Thread(KeyFunc,1,PRIORITY_NORMAL,true);

    Exit(0);
}

void KeyFunc(ulong_t arg){
    Put_Cursor(0,0);
    Set_Current_Attr(ATTRIB(BLACK, GREEN|BRIGHT));
    Print("\t\t\tWelcome to GeekOS!\n\t\t\t  By Gunasekaran\n");
    Set_Current_Attr(ATTRIB(BLACK, GRAY));
    Print("\n\n\tSimple Editor!\t(To stop receiving, press CTRL+d)\n\n");
    Keycode c = 0;
    char char_stream[1024];
    int row,col,previous_row,previous_col_end,starting_row,starting_col,stream_ptr=0;
    Get_Cursor(&starting_row,&starting_col);
    c = Wait_For_Key();
    while((c & KEY_SPECIAL_FLAG) == 1 || (c & KEY_CTRL_FLAG) == 0 || (char)c != 'd')
    {
        if((c & KEY_RELEASE_FLAG) == 0)
        {
            if(c == 276);
            else if(c == ASCII_BS){
                Get_Cursor(&row,&col);
                if(col == 0 && row == starting_row){
                    char_stream[0] = '\0';
                }
                else if(col == 0){
                    Put_Cursor(previous_row,previous_col_end);
                    stream_ptr = stream_ptr - 1;
                    char_stream[stream_ptr] = '\0';
                }
                else{
                    Put_Cursor(row,col-1);
                    Print(" ");
                    Get_Cursor(&row,&col);
                    Put_Cursor(row,col-1);
                    stream_ptr = stream_ptr - 1;
                    char_stream[stream_ptr] = '\0';
                }
            }
            else if(c == 13){
                Get_Cursor(&previous_row,&previous_col_end);
                Print("\n");
                char_stream[stream_ptr++] = '\n';
                char_stream[stream_ptr] = '\0';
            }
            else if(c == 8201){
                Print("\t");
                char_stream[stream_ptr++] = '\t';
                char_stream[stream_ptr] = '\0';
            }
            else{
                Print("%c", c);
                char_stream[stream_ptr++] = c;
                char_stream[stream_ptr] = '\0';
            }
        }
        c = Wait_For_Key();
    }
    Clear_Screen();
    Put_Cursor(0,0);
    Print("You have entered: \n");
    Print("%s",char_stream);
    Exit(0);
}
