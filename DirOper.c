/**********************************************************************
* 版权所有 (C)2014, Zhou Zhaoxiong。
*
* 文件名称： DirOper.c
* 文件标识：无
* 内容摘要：用于进行目录的相关操作
* 其它说明：无
* 当前版本：V1.0
* 作   者：周兆熊
* 完成日期：20140711
*
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
 
 
// 数据类型
typedef unsigned char UINT8;
typedef signed    int INT32;
typedef UINT8         BOOL;
 
 
// 宏定义
#define _S_IFDIR S_IFDIR
#define TRUE     (BOOL)1
#define FALSE    (BOOL)0
 
 
// 函数声明
BOOL MyDirExists(UINT8 *pszDirectory);
void MyDirCreate(UINT8*pszDirectory);
BOOL MyForceDirCreate(UINT8 *pszDirectory);
 
 
/**********************************************************************
* 功能描述：判断目录是否存在                                        
* 输入参数： Directory-目录名称                                    
* 输出参数：无                                                      
* 返 回 值： 1 - 存在
             0 - 不存在                                             
* 其它说明：无                                                      
* 修改日期       版本号     修改人        修改内容
* -----------------------------------------------------------------------            
* 20140714       V1.0     ZhouZhaoxiong     创建                       
* ------------------------------------------------------------------------------*/
BOOL MyDirExists(UINT8 *pszDirectory)
{
    struct stat st;
    BOOL   bResult           = FALSE;
    UINT8  cSep              = '/';
    UINT8  szDirectory[256]  = {0};
    UINT8  *pBegin           = NULL;
 
    // 对输入指针进行异常判断
    if (NULL == pszDirectory)
    {
       printf("MyDirExists: input parameter is NULL!\n");
        return TRUE;
    }
   
    strncpy((char*)szDirectory, (char *)pszDirectory, sizeof(szDirectory) - 1);
   
    if(szDirectory[strlen(szDirectory)-1] == cSep)         // 双目录符号
    {
        pBegin =strrchr(szDirectory, (int)cSep);
        if (NULL != pBegin)
        {
            pBegin[0] = 0;
        }
    }
   
    if (stat(szDirectory, &st) == 0)               // 检查目录是否存在
    {
        bResult =((st.st_mode & _S_IFDIR) == _S_IFDIR);
    }
    else
    {
        bResult = 0;
    }
   
    return bResult;
}
 
 
 
/**********************************************************************
* 功能描述：创建目录                                                
* 输入参数： pszDirectory-目录名称                             
* 输出参数：无                                                      
* 返 回 值： 1-创建成功 0-创建失败                                                
* 其它说明：无                                                      
* 修改日期     版本号       修改人         修改内容                     
*------------------------------------------------------------------                  
* 20140714      V1.0    Zhou Zhaoxiong       创建                       
*------------------------------------------------------------------- */
void MyDirCreate(UINT8 *pszDirectory)
{
    UINT8 szRunCmd[256] ={0};
      
    // 判断输入参数是否为空
    if (pszDirectory == NULL)
    {
        printf("MyDirCreate: input parameter isNULL!\n");
        return;
    }
   
    sprintf(szRunCmd,  "mkdir -p %s", pszDirectory);
    system(szRunCmd);
      
    return;
}
 
 
/**********************************************************************
* 功能描述：强制创建目录                                            
* 输入参数： Directory-目录名称                             
* 输出参数：无                                                      
* 返 回 值：无                                                      
* 其它说明：无                                                      
* 修改日期        版本号        修改人                修改内容                     
*-----------------------------------------------------------------------                   
* 20140714        V1.0      ZhouZhaoxiong              创建                       
*------------------------------------------------------------------------------*/
BOOL MyForceDirCreate(UINT8 *pszDirectory)
{
    UINT8 *pszStart = NULL;
    UINT8 *pszEnd   = NULL;
 
    UINT8  szDirectory[256] = {0};
    UINT8  cSep = '/';
 
    UINT8  szTmp[2]={0};
      
    if (NULL == pszDirectory)
    {
       printf("MyForceDirCreate: input parameter is NULL!\n");
       return FALSE;
    }
   
    if(pszDirectory[strlen(pszDirectory)-1] != cSep)     // 没有目录结束
    {
        szTmp[0] = cSep;
        szTmp[1] = 0;
        strcat((char*)pszDirectory, szTmp);
    }
      
    pszStart =(char*)pszDirectory + 1;
    pszEnd  = pszStart;
      
    while (pszEnd)
    {
        if (!(pszEnd =strchr(pszStart,(int)'/')))
        {
            break;
        }
             
        memset(szDirectory,0x00, sizeof(szDirectory));
        strncpy(szDirectory,pszDirectory, pszEnd-pszDirectory);
        pszStart = pszEnd +1;

        if(!MyDirExists((UINT8*)szDirectory))
        {
           MyDirCreate(szDirectory);
        }
    }
   
    return TRUE;
}
 
 
/****************************************************************
* 功能描述: 主函数                                           
* 输入参数: 无                                                
* 输出参数: 无                                               
* 返 回 值: 0-执行成功 -1-执行失败                           
* 其他说明: 无                                               
* 修改日期         版本号        修改人        修改内容
* --------------------------------------------------------------
* 20140714         V1.0      Zhou Zhaoxiong     创建
****************************************************************/
INT32 main(void)
{
    UINT8  szDirectory[256] = {0};
 
    memcpy(szDirectory,"/zhouzx/TestDir", strlen("/zhouzx/TestDir"));
 
    // 先检测目录是否存在, 如果不存在, 则创建目录
    if (!MyDirExists(szDirectory))
    {
        printf("Now,create directory %s\n", szDirectory);
        if (MyForceDirCreate(szDirectory))
        {
             printf("Create directory %ssuccessfully!\n", szDirectory);
        }
         else
         {
             printf("Create directory %sfailed!\n", szDirectory);
             return -1;
         }
    }
    else
    {
        printf("%salready exists!\n", szDirectory);
    }
 
    return 0;
}
