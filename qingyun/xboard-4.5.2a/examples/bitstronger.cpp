/*! \file BitStronger.cpp
@brief ������main�������ڣ� 
*/
 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "constant.h"
#include "board.h"
#include "searcher.h"
#include "hashnum.h"
#include "openbook.h"

#ifdef _WIN32

#include <windows.h>

char workpath[256], hashpath[256], bookpath[256];
///����ϵͳ��·���ָ��� 
const int BACK_SLASH_CHAR = '\\';
///�õ����湤��·�� 
void getpath()
{
    GetModuleFileName(NULL, workpath, sizeof(workpath) - 1);
    *(strrchr(workpath, BACK_SLASH_CHAR) + 1) = 0;
    strcpy(hashpath, workpath);
    strcat(hashpath, "data\\Zobrist.bin");
    strcpy(bookpath, workpath);
    strcat(bookpath, "data\\OpenBook.bin");
}

#else

char workpath[256], hashpath[256], bookpath[256];
///����ϵͳ��·���ָ��� 
const int BACK_SLASH_CHAR = '/';
///�õ����湤��·�� 
void getpath()
{
/*FIXME: change the hashpath and bookpath*/
    strcpy(hashpath,"/media/disk/tool/gloox-1.0/data/zobrist.bin");
    strcpy(bookpath,"/media/disk/tool/gloox-1.0/data/openbook.bin");        
#if 0
    workpath[0] = '.';
    workpath[1] = BACK_SLASH_CHAR;
    workpath[2] = '\0';
    strcpy(hashpath, workpath);
    strcat(hashpath, "Zobrist.bin");
    strcpy(bookpath, workpath);
    strcat(bookpath, "OpenBook.bin");
#endif
}

#endif

void bitstronger_init()
{
    
    getpath();
    Hashnum::InitHash();
    OpenBook::InitBook();

}
void bitstronger_exit()
{
    Hashnum::ClearHash();
    OpenBook::ClearBook();

}
Board g_curboard;
/*
short_fen : it is not a complete fen, it only contains the board description
whoseturn: only 2 value, 'b' or 'w'. 'b' means "black", 'w' means "white".
*/
void bitstronger_setposition(std::string short_fen,char whoseturn)
{
//    printf("bitstronger_setposition,fen=%s\n",short_fen.c_str());
    g_curboard.FromFen(short_fen.c_str(), whoseturn, 0, NULL);

}
std::string bitstronger_move()
{
    return Searcher::GetBestmove(g_curboard, g_curboard.player);
}
///���غ���
/*!
��������ģ��ά����һ��ȫ��Ψһ�ĵ������ʵ�������ĺ�������Ϣѭ��������ƣ�ͨ������UCCIͨѶģ��ʵʱ�ؼ����������ĸ���ָ���ָ��Ҫ������������ģ��Э��������Ȼ������ķ���������������
*/ 
int _bitstronger(int argc, char *argv[])
{
#if 0
    //UCCIָ������ 
    UcciCommEnum uce;
    //UCCIָ������ 
    UcciCommStruct ucs;
    //��ǰ���� 
    Board curboard;
    
    getpath();
        
    if (BootLine() == UCCI_COMM_UCCI)
    {
        printf("id name BitStronger 0.06\n");
        fflush(stdout);
        printf("id copyright 2006 B.I.T.\n");
        fflush(stdout);
        printf("id author BitStronger Authors\n");
        fflush(stdout);
        printf("id user Our Users\n");
        fflush(stdout);
        printf("ucciok\n");
        fflush(stdout);
        
        Hashnum::InitHash();
        OpenBook::InitBook();

        // �����ǽ���ָ����ṩ�Բߵ�ѭ����
        uce = UCCI_COMM_NONE;
        while (uce != UCCI_COMM_QUIT)
        {
            uce = IdleLine(ucs, TRUE);
            switch (uce)
            {
            case UCCI_COMM_ISREADY:
                printf("readyok\n");
                fflush(stdout);
                break;
            case UCCI_COMM_STOP:
                printf("nobestmove\n");
                fflush(stdout);
                break;
            case UCCI_COMM_POSITION:
                curboard.FromFen(ucs.Position.szFenStr, ucs.Position.nMoveNum, ucs.Position.lpdwCoordList);
                break;
            case UCCI_COMM_BANMOVES:
                //Nothing
                break;
            case UCCI_COMM_SETOPTION:
                //Nothing
                break;
            case UCCI_COMM_GO:
            case UCCI_COMM_GOPONDER:
                Searcher::GetBestmove(curboard, curboard.player);
                break;
            }
        }
    }
    printf("bye\n");
    fflush(stdout);
    
    Hashnum::ClearHash();
    OpenBook::ClearBook();
#endif    
    return 0;
}
