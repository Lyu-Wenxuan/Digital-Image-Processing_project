#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <malloc.h>
#include <math.h>
#include <vector>
#include <stack>
#include<algorithm>
#define pi 3.1415926????????????????/
#define M_ANGLE M_PI / 180
#pragma pack(2)   //ȫ����ģʽ
using namespace std;
typedef unsigned char BYTE;
typedef struct
{
    unsigned char bfType[2];
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} bitmapFileHeader; //λͼ�ļ�ͷ

typedef struct
{
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPixPerMeter;
    unsigned int biYPixPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} bitmapInfoHeader; //λͼ��Ϣͷ

typedef struct
{
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
} rgbQUAD; //��ɫ��

typedef struct
{
    bitmapFileHeader bfHeader;
    bitmapInfoHeader biHeader;
    rgbQUAD palette[256];
    unsigned char *imgData;
} bmp;

int write(char savePath[],bmp b)
{
    FILE *f=fopen(savePath,"wb");
    if(f==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f);
    if(b.biHeader.biBitCount==8)
    {
        fwrite(&b.palette,1024,1,f);
    }
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f);
    fclose(f);
    cout<<"�ɹ������"<<endl;
    return 0;

}

//�ҶȻ�
void gray(bmp b,int lineBytes_new,int lineBytes,int height,int width,unsigned char *imageData)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            //��ÿһ�����ض�����ʽy=B*0.299+G*0.587+R*0.114����ת��
            b.imgData[lineBytes_new*i+j]=imageData[lineBytes*i+j*3]*0.299+imageData[lineBytes*i+j*3+1]*0.587+imageData[lineBytes*i+j*3+2]*0.114;
        }
    }

    write("D:/ͼ����/test��/image/gray.bmp",b);
}

//��ɫ
void reverse(bmp b,int lineBytes_new,int lineBytes,int height,int width,unsigned char*imageData)
{
    gray(b,lineBytes_new,lineBytes,height,width,imageData);
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes_new*i+j]=255-b.imgData[lineBytes_new*i+j];
        }
    }
    write("D:/ͼ����/test��/image/reverse.bmp",b);
}

//��ɫ����
void blue(bmp b,int lineBytes_new,int lineBytes,int height,int width,unsigned char*imageData)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes_new*i+j]=imageData[lineBytes*i+j*3]*1+imageData[lineBytes*i+j*3+1]*0+imageData[lineBytes*i+j*3+2]*0;
        }
    }
    write("D:/ͼ����/test��/image/b.bmp",b);
}

//��ɫ����
void green(bmp b,int lineBytes_new,int lineBytes,int height,int width,unsigned char*imageData)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes_new*i+j]=imageData[lineBytes*i+j*3]*0+imageData[lineBytes*i+j*3+1]*1+imageData[lineBytes*i+j*3+2]*0;
        }
    }
    write("D:/ͼ����/test��/image/g.bmp",b);
}

//��ɫ����
void red(bmp b,int lineBytes_new,int lineBytes,int height,int width,unsigned char*imageData)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes_new*i+j]=imageData[lineBytes*i+j*3]*0+imageData[lineBytes*i+j*3+1]*0+imageData[lineBytes*i+j*3+2]*1;
        }
    }
    write("D:/ͼ����/test��/image/r.bmp",b);
}

int exp1()
{
    FILE *fp;
    if((fp=fopen("D:/ͼ����/test��/image/rgb.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);//��λͼ��Ϣͷ
    int imSize=biHeader.biSizeImage;//�ļ���С
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;//��ɫλ��
    int lineBytes=(width*bitCount+31)/32*4;//���ֽ���

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);//��λͼ����
    fclose(fp);

    bmp b;
    int i;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.bfHeader.bfOffBits=1078;  //����ͼ������ƫ��λ�ã�bfOffBits = sizeof (BITMAPFILEHEADER) + NumberOfRGBQUAD * sizeof (RGBQUAD)  + sizeof (BITMAPINFOHEADER);
    b.biHeader.biBitCount=8;  //�ı�ͼ��λ��
    int lineBytes_new=(width*8+31)/32*4; //���¼���ÿ�������ֽ�
    b.biHeader.biSizeImage=lineBytes_new*height; //�ı�ͼ�����ݴ�С
    b.bfHeader.bfSize=1078+b.biHeader.biSizeImage; //�ı��ļ����ݴ�С
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*b.biHeader.biSizeImage);
    memset(b.imgData,0,sizeof(unsigned char)*b.biHeader.biSizeImage);

    for(i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }

    gray(b,lineBytes_new,lineBytes,height,width,imageData);
    cout<<"����ҶȻ�ͼƬ�ɹ���"<<endl;

    reverse(b,lineBytes_new,lineBytes,height,width,imageData);
    cout<<"�����ɫͼƬ�ɹ���"<<endl;

    red(b,lineBytes_new,lineBytes,height,width,imageData);
    cout<<"�����ɫͨ������ͼƬ�ɹ���"<<endl;

    green(b,lineBytes_new,lineBytes,height,width,imageData);
    cout<<"�����ɫͨ������ͼƬ�ɹ���"<<endl;

    blue(b,lineBytes_new,lineBytes,height,width,imageData);
    cout<<"�����ɫͨ������ͼƬ�ɹ���"<<endl;

    free(imageData);
    free(b.imgData);
    return 0;
}

typedef struct tagBITMAPFILEHEADER
{
    //unsigned char bfType[2];        // 19778��������BM�ַ�������Ӧ��ʮ������Ϊ0x4d42,ʮ����Ϊ19778��������bmp��ʽ�ļ�
    unsigned int   bfSize;        // �ļ���С ���ֽ�Ϊ��λ(2-5�ֽ�)
    unsigned short bfReserved1;   // ��������������Ϊ0 (6-7�ֽ�)
    unsigned short bfReserved2;   // ��������������Ϊ0 (8-9�ֽ�)
    unsigned int   bfOffBits;     // ���ļ�ͷ���������ݵ�ƫ��  (10-13�ֽ�)
} BITMAPFILEHEADER;

//ͼ����Ϣͷ�ṹ��
typedef struct tagBITMAPINFOHEADER
{
    unsigned int    biSize;          // �˽ṹ��Ĵ�С (14-17�ֽ�)
    long            biWidth;         // ͼ��Ŀ�  (18-21�ֽ�)
    long            biHeight;        // ͼ��ĸ�  (22-25�ֽ�)
    unsigned short  biPlanes;        // ��ʾbmpͼƬ��ƽ��������Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1 (26-27�ֽ�)
    unsigned short  biBitCount;      // һ������ռ��λ����һ��Ϊ24   (28-29�ֽ�)
    unsigned int    biCompression;   // ˵��ͼ������ѹ�������ͣ�0Ϊ��ѹ���� (30-33�ֽ�)
    unsigned int    biSizeImage;     // ����������ռ��С, ���ֵӦ�õ��������ļ�ͷ�ṹ��bfSize-bfOffBits (34-37�ֽ�)
    long            biXPelsPerMeter; // ˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (38-41�ֽ�)
    long            biYPelsPerMeter; // ˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (42-45�ֽ�)
    unsigned int    biClrUsed;       // ˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ����� (46-49�ֽ�)
    unsigned int    biClrImportant;  // ˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��(50-53�ֽ�)
} BITMAPINFOHEADER;

//24λͼ������Ϣ�ṹ��,����ɫ��
typedef struct _PixelInfo
{
    unsigned char rgbBlue;   //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbGreen;  //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbRed;    //����ɫ�ĺ�ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbReserved;// ����������Ϊ0
} PixelInfo;

BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
PixelInfo *temp_color;

unsigned char *pBmpBuf;
PixelInfo *pColorTable;
unsigned char *pBmpBuf1;

int biBitCount;
int bmpHeight;
int bmpWidth;

int exp2()
{
    FILE* fp;   //Դ�ļ�
    FILE* fp_zhifang; //ֱ��ͼ
    FILE* fp_zhifang_2; //���⻯���ֱ��ͼ
    FILE* fp_balanced; //���⻯��ĻҶ�ͼ

    fp = fopen("D:/ͼ����/test��/image/dim.bmp", "rb");//��ȡͬĿ¼�µ�image.bmp�ļ���
    if(fp == NULL)
    {
        printf("can not open file��\n");
        return -1;
    }
    //������ȶ�ȡbifType������C���Խṹ��Sizeof������򡪡�������ڲ���֮�ͣ��Ӷ����¶��ļ���λ
    unsigned short fileType;
    fread(&fileType, sizeof(unsigned short), 1, fp);
    if (fileType = 0x4d42)
    {
        cout<<"�ɹ����ļ�!" <<endl;
        cout<<"�ɹ����ͼƬ��"<<endl;

        fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
        //showBmpHead(fileHeader);
        fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
        //showBmpInfoHead(infoHeader);

        bmpHeight = infoHeader.biHeight;    //��ȡbmp�ļ������ش�С�������
        bmpWidth = infoHeader.biWidth;
        biBitCount = infoHeader.biBitCount;

        if(biBitCount==8)
        {
            temp_color = (PixelInfo*)malloc(256*sizeof(PixelInfo));
            fread(temp_color, sizeof(PixelInfo), 256, fp);
        }

        int LineByte=(bmpWidth*biBitCount/8+3)/4*4;  //��֤ÿһ���ֽ�����Ϊ4��������
        pBmpBuf = (unsigned char *)malloc(LineByte*bmpHeight*sizeof(unsigned char)); //��ȡ�洢ͼ����������Ҫ�Ŀռ�
        fread(pBmpBuf, LineByte*bmpHeight, 1, fp);


        //�Ҷ�ͼ����
        int LineByte1=(bmpWidth*8/8+3)/4*4;  //��֤ÿһ���ֽ�����Ϊ4��������

        //�޸��ļ�ͷ,������������Ҫ�޸ģ��ֱ�ΪbfSize��bfOffBits
        fileHeader.bfSize=14+40+256*sizeof(PixelInfo)+LineByte1*bmpHeight;//�޸��ļ���С
        fileHeader.bfOffBits=14+40+256*sizeof(PixelInfo);//�޸�ƫ���ֽ���

        //�޸���Ϣͷ��������������Ҫ�޸ģ�1��λbiBitCount:���ͼΪ24 ��Ӧ�ĳ�8����һ����biSizeImage:����ÿ������ռλ���ı仯������λͼ���ݵĴ�С�����仯
        infoHeader.biBitCount=8;    //��ÿ���ص�λ����Ϊ8
        infoHeader.biSizeImage=LineByte1*bmpHeight;//�޸�λͼ���ݵĴ�С

        pColorTable = (PixelInfo*)malloc(256*sizeof(PixelInfo));
        for (int  i = 0; i < 256; i++ )
            pColorTable[i].rgbRed = pColorTable[i].rgbGreen = pColorTable[i].rgbBlue = i; //����ɫ�����B��G��R��������ȣ��ҵ�������ֵ

        int grayCount[256] = {0}; //ͳ�ƻҶ�Ƶ�ʵ�����
        if (biBitCount==8)  //��ֹ������Ǹ��Ҷ�ͼ
        {

            pBmpBuf1 = pBmpBuf;
            for(int i=0; i<bmpHeight; i++)
                for(int j=0; j<bmpWidth; j++)
                {
                    unsigned char *pb1;
                    pb1=pBmpBuf+i*LineByte+j;
                    int y=*pb1;   //ͳ�ƻҶ�ֵ
                    grayCount[y]++;
                }
        }
        else
        {
            pBmpBuf1 = (unsigned char *)malloc(LineByte1*bmpHeight*sizeof(unsigned char));;  //��ȡδ���洢λͼ������Ҫ�Ŀռ�
            for(int i=0; i<bmpHeight; i++)
                for(int j=0; j<bmpWidth; j++)
                {
                    unsigned char *pb1,*pb2;
                    pb1=pBmpBuf+i*LineByte+j*3;
                    int y=*(pb1)*0.299+*(pb1+1)*0.587+*(pb1+2)*0.114;   //��ÿһ�����ض�����ʽy=B*0.299+G*0.587+R*0.114����ת��
                    pb2=pBmpBuf1+i*LineByte1+j;
                    *pb2=y;
                    grayCount[y]++;
                }

        }

        //��ɫͼ����
        for(int i=0; i<bmpHeight; i++)
            for(int j=0; j<bmpWidth; j++)
            {
                unsigned char *pb2;
                pb2=pBmpBuf1+i*LineByte1+j;
                *pb2=255-*pb2;  //ʹ��255��ȥԭ���ĻҶ�ֵ
            }

        // �Ҷ�ֱ��ͼ���⻯
        int gray_balanced[256] = {0};
        int grayCount_balanced[256] = {0};
        for(int i=0; i<256; i++)
        {
            double temp = 0;
            for (int j=0; j<i; j++)
            {
                temp = temp + grayCount[j];
            }
            temp = temp / (bmpHeight*bmpWidth);
            gray_balanced[i] = (int)(255*temp + 0.5);
        }

        fp_balanced = fopen("D:/ͼ����/test��/image/dim_gray_balanced.bmp", "wb");
        if(fp_balanced == NULL) return -1;

        for(int i=0; i<bmpHeight; i++)
            for(int j=0; j<bmpWidth; j++)
            {
                unsigned char *pb2;
                pb2=pBmpBuf1+i*LineByte1+j;
                *pb2=255-*pb2;  //ʹ��255��ȥԭ���ĻҶ�ֵ
                *pb2=gray_balanced[*pb2];
                grayCount_balanced[*pb2]++;
            }

        fwrite(&fileType, sizeof(unsigned short), 1, fp_balanced);
        fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp_balanced);    //���޸ĺ���ļ�ͷ����fp_balanced��
        fwrite(&infoHeader,sizeof(BITMAPINFOHEADER), 1, fp_balanced);  //���޸ĺ����Ϣͷ����fp_balanced;
        fwrite(pColorTable,sizeof(PixelInfo),256,fp_balanced); //����ɫ��д��fp_balanced;
        fwrite(pBmpBuf1, LineByte1*bmpHeight, 1, fp_balanced); //дͼ������


        //�Ҷ�ֱ��ͼ����
        fp_zhifang = fopen("D:/ͼ����/test��/image/dim_zhifang.bmp", "wb");
        if(fp_zhifang == NULL) return -1;

        int bmp_zf_width = 256;
        int bmp_zf_height = 256;
        int LineByte_zf=(bmp_zf_width*8/8+3)/4*4;  //��֤ÿһ���ֽ�����Ϊ4��������
        unsigned char *pBmpBuf_zf = (unsigned char *)malloc(LineByte_zf*bmp_zf_height*sizeof(unsigned char));;  //��ȡδ���洢λͼ������Ҫ�Ŀռ�


        int max = grayCount[0];
        for(int i=0; i<256; i++)    //��ȡ���ĸ��ʵ����صĸ���
        {
            //printf("%d ", grayCount[i]);
            if(grayCount[i] > max)
            {
                max = grayCount[i];
            }
        }

        for(int i=0; i<256; i++)    //ͳ��ÿ�����صĸ���
        {
            grayCount[i] = ((double)grayCount[i]) / (max) * 256;
        }
        for(int i=0; i<bmp_zf_height; i++) //ֱ��ͼ��λͼ��������
            for(int j=0; j<bmp_zf_width; j++)
            {
                unsigned char *pb2;
                pb2=pBmpBuf_zf+i*LineByte_zf+j;
                *pb2 = grayCount[j]>i?0:255;
            }

        fwrite(&fileType, sizeof(unsigned short), 1, fp_zhifang); //д�ļ�����
        BITMAPFILEHEADER fileHeader_zf = fileHeader;
        BITMAPINFOHEADER infoHeader_zf = infoHeader;

        fileHeader_zf.bfSize=14+40+256*sizeof(PixelInfo)+LineByte_zf*bmp_zf_height;//�޸��ļ���С
        infoHeader_zf.biSizeImage=LineByte_zf*bmp_zf_height;//�޸�λͼ���ݵĴ�С
        infoHeader_zf.biHeight=bmp_zf_height;
        infoHeader_zf.biWidth=bmp_zf_width;

        fwrite(&fileHeader_zf, sizeof(BITMAPFILEHEADER), 1, fp_zhifang); //дλͼ�ļ�ͷ
        fwrite(&infoHeader_zf, sizeof(BITMAPINFOHEADER), 1, fp_zhifang); //дλͼ��Ϣͷ
        fwrite(pColorTable, sizeof(PixelInfo), 256, fp_zhifang); //д��ɫ��
        fwrite(pBmpBuf_zf, LineByte_zf*bmp_zf_height, 1, fp_zhifang); //дͼ������


        fp_zhifang_2 = fopen("D:/ͼ����/test��/image/dim_zhifang_balanced.bmp", "wb");
        if(fp_zhifang_2 == NULL) return -1;

        max = grayCount_balanced[0];
        for(int i=0; i<256; i++)    //��ȡ���ĸ��ʵ����صĸ���
        {
            if(grayCount_balanced[i] > max)
            {
                max = grayCount_balanced[i];
            }
        }
        for(int i=0; i<256; i++)    //ͳ��ÿ�����صĸ���
        {
            grayCount_balanced[i] = ((double)grayCount_balanced[i]) / (max) * 256;
        }
        for(int i=0; i<bmp_zf_height; i++) //ֱ��ͼ��λͼ��������
            for(int j=0; j<bmp_zf_width; j++)
            {
                unsigned char *pb2;
                pb2=pBmpBuf_zf+i*LineByte_zf+j;
                *pb2 = grayCount_balanced[j]>i?0:255;
            }

        fwrite(&fileType, sizeof(unsigned short), 1, fp_zhifang_2); //д�ļ�����
        fwrite(&fileHeader_zf, sizeof(BITMAPFILEHEADER), 1, fp_zhifang_2); //дλͼ�ļ�ͷ
        fwrite(&infoHeader_zf, sizeof(BITMAPINFOHEADER), 1, fp_zhifang_2); //дλͼ��Ϣͷ
        fwrite(pColorTable, sizeof(PixelInfo), 256, fp_zhifang_2); //д��ɫ��
        fwrite(pBmpBuf_zf, LineByte_zf*bmp_zf_height, 1, fp_zhifang_2); //дͼ������

        fclose(fp);
        fclose(fp_balanced);
        fclose(fp_zhifang);
        fclose(fp_zhifang_2);
    }
    return 0;
}

int sum(unsigned char **num, int m, int n)
{
    int sum=0;
    int i,j;
    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            sum+=num[i][j];
        }
    }
    return sum;
}//���

unsigned char median(unsigned char num[],int m)
{
    int i,j;
    unsigned char temp;
    for(i=0; i<m; i++)
    {
        for(j=i+1; j<m; j++)
        {
            if(num[j]<num[i])
            {
                temp=num[i];
                num[i]=num[j];
                num[j]=temp;
            }
        }
    }
    return num[m/2];
}//����ֵ

int exp3_1()
{

    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);

    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }

    int i,j;
    unsigned char m=3,n=3;
    unsigned char **data=(unsigned char**)malloc(n*sizeof(unsigned char*));

    for(i=0; i<n; i++)
        data[i]=(unsigned char*)malloc(m);

    int x,y;
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
            if(j-(m-1)/2>=0&&j+m/2<width&&i-(n-1)/2>=0&&i+n/2<height) //ȷ���ڱ߽��ڱ���
            {
                for(y=0; y<n; y++)
                {
                    for(x=0; x<m; x++)
                    {
                        data[y][x]=imageData[lineBytes*(i-(n-1)/2+y)+j-(m-1)/2+x];//��¼ģ����ÿ�����ص�����
                    }
                }
                b.imgData[lineBytes*i+j]=sum(data,m,n)/(m*n);//���ֵ
            }
            else
                b.imgData[lineBytes*i+j]=imageData[lineBytes*i+j];
        }
    }

    char savePath[]="D:/ͼ����/test��/image/average.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
        return -2;
    }

    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("ƽ���˲�ͼƬ�ɹ������\n");
    return 0;
}
int exp3_2()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/noise2.bmp";

    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);

    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }

    int i,j;
    unsigned char m=3,n=3;
    unsigned char **data=(unsigned char**)malloc(n*sizeof(unsigned char*));
    unsigned char *row=(unsigned char*)malloc(m);//��
    unsigned char *col=(unsigned char*)malloc(n);//��
    for(i=0; i<n; i++)
        data[i]=(unsigned char*)malloc(m);

    //����ֵ�˲�
    int x,y;
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
            if(j-(m-1)/2>=0&&j+m/2<width&&i-(n-1)/2>=0&&i+n/2<height) //j>=1,j<width-1
            {
                for(y=0; y<n; y++)
                {
                    for(x=0; x<m; x++)
                    {
                        row[x]=imageData[lineBytes*(i-(n-1)/2+y)+j-(m-1)/2+x];//��ֵ

                    }
                    col[y]=median(row,m);//��ģ��ÿ�е���ֵ
                }
                b.imgData[lineBytes*i+j]=median(col,n);//��ģ��ÿ����ֵ����ֵ
            }
            else
            {
                continue;
            }

        }
    }

    char savePath[]="D:/ͼ����/test��/image/median.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!\n");
        return -2;
    }

    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("��ֵ�˲�ͼƬ�ɹ������\n");
    return 0;
}

double fmax(double a,double b)
{
    if(a>b)
        return a;
    else
        return b;
}
int exp4_1()
{

    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);
    char *savePath=NULL;
    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }
    int xOffset=100,yOffset=100;//x,yֵ
    //ƽ��
    if(width<xOffset||height<yOffset)
        return -1;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(i+yOffset<height&&i+yOffset>=0&&j+xOffset<width&&j+xOffset>=0)
                b.imgData[lineBytes*(i+yOffset)+j+xOffset]=imageData[lineBytes*i+j];
        }
    }
    savePath="D:/ͼ����/test��/image/lena_move.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("ƽ��ͼƬ�ɹ������\n");
    return 0;


}
int exp4_2()
{

    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);
    char *savePath=NULL;
    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }

    //��ͼ�������ת
    double angle=pi/3;//�Ƕ�
    double half_w=width/2.0,half_h=height/2.0;
    double cosa=cos(angle),sina=sin(angle);
    //x=x0cos(angle)+y0sin(angle),y=-x0sin(angle)+y0cos(angle)
    //��ԭͼ����ĸ�����任���λ��
    double x1=-half_w*cosa+ half_h*sina,y1=half_w*sina+half_h*cosa;
    double x2=half_w*cosa+half_h*sina,y2=-half_w*sina+half_h*cosa;
    double x3=-half_w*cosa-half_h*sina,y3=half_w*sina-half_h*cosa;
    double x4=half_w*cosa-half_h*sina,y4=-half_w*sina-half_h*cosa;

    //����ͼ��Ĵ�С
    int width_new=(int)(fmax(fabs(x4-x1),fabs(x3-x2))+0.5);
    int height_new=(int)(fmax(fabs(y4-y1),fabs(y3-y2))+0.5);

    int lineBytes_new=(width_new*bitCount+31)/32*4;
    b.biHeader.biWidth=width_new;
    b.biHeader.biHeight=height_new;
    b.biHeader.biSizeImage=lineBytes_new*height_new;
    b.bfHeader.bfSize=1078+lineBytes_new*height_new;//bfSize = biSizeImage + 54
    b.imgData=(unsigned char*)realloc(b.imgData,lineBytes_new*height_new*sizeof(unsigned char));
    memset(b.imgData,0,lineBytes_new*height_new*sizeof(unsigned char));

    //������������
    double const1=-width_new*cosa/2.0+height_new*sina/2.0+width/2.0;
    double const2=-width_new*sina/2.0-height_new*cosa/2.0+height/2.0;

    int x0,y0;
    for(int i=0; i<height_new; i++)
    {
        for(int j=0; j<width_new; j++)
        {
            //��ǰͼ�����ص���ԭͼ���ж�Ӧ�����ص�
            x0=(int)(j*cosa-i*sina+const1+0.5);
            y0=(int)(j*sina+i*cosa+const2+0.5);
            if(x0>=0&&x0<width&&y0>=0&&y0<height)
            {
                b.imgData[lineBytes_new*i+j]=imageData[lineBytes*y0+x0];
            }
        }
    }
    savePath="D:/ͼ����/test��/image/lena_revolve.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("��תͼƬ�ɹ������\n");
    return 0;
}
int exp4_3()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);
    char *savePath=NULL;
    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }
    //����
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            //���ҷ�ת
            b.imgData[lineBytes*i+j]=imageData[lineBytes*i+(width-1-j)];
        }
    }
    savePath="D:/ͼ����/test��/image/lena_mirror.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("����ͼƬ�ɹ������\n");
    return 0;

}
int exp4_4()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    //�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);
    char *savePath=NULL;
    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }
    //����
    double ratio=2.0;//����
    int width_new=(int)(width*ratio+0.5),height_new=(int)(height*ratio+0.5);//����ͼ��Ĵ�С
    int lineBytes_new=(width_new*bitCount+31)/32*4;
    b.biHeader.biWidth=width_new;
    b.biHeader.biHeight=height_new;
    b.biHeader.biSizeImage=height_new*lineBytes_new*sizeof(unsigned char);
    b.bfHeader.bfSize=1078+height_new*lineBytes_new*sizeof(unsigned char);
    b.imgData=(unsigned char*)realloc(b.imgData,height_new*lineBytes_new*sizeof(unsigned char));
    memset(b.imgData,0,height_new*lineBytes_new*sizeof(unsigned char));
    int x0,y0;
    for(int i=0; i<height_new; i++)
    {
        for(int j=0; j<width_new; j++)
        {
            x0=(int)(j/ratio);
            y0=(int)(i/ratio);
            if(x0>=0&&x0<width&&y0>=0&&y0<height)
            {
                b.imgData[lineBytes_new*i+j]=imageData[lineBytes*y0+x0];
            }
        }
    }
    savePath="D:/ͼ����/test��/image/lena_scale.bmp";
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    fwrite(&b.palette,1024,1,f_save);
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);

    free(imageData);
    free(b.imgData);
    printf("����ͼƬ�ɹ������\n");
    return 0;

}

//д��ֵ��ͼƬ
void Bwrite(char *savePath,bmp b)
{
    FILE *f_save=fopen(savePath,"wb");
    if(f_save==NULL)
    {
        perror("can not open file!");
    }
    fwrite(&b.bfHeader,sizeof(bitmapFileHeader),1,f_save);
    fwrite(&b.biHeader,sizeof(bitmapInfoHeader),1,f_save);
    if(b.biHeader.biBitCount==8)
    {
        fwrite(&b.palette,1024,1,f_save);
    }
    fwrite(b.imgData,sizeof(unsigned char)*b.biHeader.biSizeImage,1,f_save);
    fclose(f_save);
    printf("��ֵ���ɹ������\n");
}
//дֱ��ͼ
void Zwrite(char *savePath2,bmp a,unsigned char *pBmpBuf_zf)
{
    FILE *f_save2=fopen(savePath2,"wb");
    if(f_save2==NULL)
    {
        perror("can not open file!");
    }
    fwrite(&a.bfHeader,sizeof(bitmapFileHeader),1,f_save2);
    fwrite(&a.biHeader,sizeof(bitmapInfoHeader),1,f_save2);
    fwrite(pBmpBuf_zf,sizeof(unsigned char)*a.biHeader.biSizeImage,1,f_save2);
    fclose(f_save2);
    printf("ֱ��ͼ�ɹ������\n");
}
//�����ֵ������
void binaryzation(int height,int width,unsigned char*imageData,bmp b,int lineBytes,int grayCount[256],int threshold)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int data=imageData[lineBytes*i+j];
            if(data>=threshold)
                b.imgData[lineBytes*i+j]=255;
            else
                b.imgData[lineBytes*i+j]=0;
            grayCount[data]++;
        }
    }
}
//����ֱ��ͼ����
void zhifang(int grayCount[256],int threshold,int LineByte_zf,unsigned char *pBmpBuf_zf)
{
    int max = grayCount[0];
    for(int i=0; i<256; i++)    //��ȡ���ĸ��ʵ����صĸ���
    {
        if(grayCount[i] > max)
        {
            max = grayCount[i];
        }
    }
    for(int i=0; i<256; i++)
    {
        grayCount[i]=(int)grayCount[i]*256/max;
    }
    for(int i=0; i<256; i++) //ֱ��ͼ��λͼ��������
        for(int j=0; j<256; j++)
        {
            if(j==threshold)
            {
                pBmpBuf_zf[i*LineByte_zf+j*3] = 0;
                pBmpBuf_zf[i*LineByte_zf+j*3+1] = 0;
                pBmpBuf_zf[i*LineByte_zf+j*3+2] = 255;
            }
            else
            {
                pBmpBuf_zf[i*LineByte_zf+j*3] = grayCount[j]>i?0:255;
                pBmpBuf_zf[i*LineByte_zf+j*3+1] = grayCount[j]>i?0:255;
                pBmpBuf_zf[i*LineByte_zf+j*3+2] = grayCount[j]>i?0:255;
            }

        }
}
//������ֵ����ֵ
int Qthreshold(int gray[],int th,int imSize)
{
    int small=0;
    int scount=0;//С�ڵ���th�ĸ���
    int big=0;
    int bcount=0;//����th�ĸ���
    for(int i=0; i<imSize; i++)
    {
        if(gray[i]<th)
        {
            small+=gray[i];
            scount++;
        }
        else
        {
            big+=gray[i];
            bcount++;
        }
    }
    int th_new=(small/scount+big/bcount)/2;
    if(abs(th-th_new)<=4)
        return th_new;
    else
        return Qthreshold(gray,th_new,imSize);
}
//otsu
int Otsu(unsigned char*imageData,int th,int lineBytes,int height,int width)
{
    long long small=0;
    int scount=0;//С�ڵ���th�ĸ���
    long long big=0;
    int bcount=0;//����th�ĸ���
    float p1=0;//����th�ĸ���
    float p2=0;
    float m1=0,m2=0;
    int th_new=0;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(imageData[lineBytes*i+j]<th)
            {
                small+=imageData[lineBytes*i+j];
                scount++;
            }
            else
            {
                big+=imageData[lineBytes*i+j];
                bcount++;
            }
        }
    }
    if(scount==0||bcount==0)//��ֹ��ĸΪ��
    {
        return 0;
    }
    p1=(float)bcount/(width*height);//���ָ���
    int t=(int)(p1*10000+0.5f);
    p1=(float)t/10000;
    p2=1-p1;
    m1=(float)big/bcount;//ƽ���Ҷ�
    m2=(float)small/scount;
    th_new=(int)(p1*p2*pow((m1-m2),2));
    return th_new;
}
int exp5()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/lena.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }

    int threshold=0;//��ֵ
    int grayCount[256] = {0}; //ͳ�ƻҶ�Ƶ�ʵ�����

    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);
    fclose(fp);
    char *savePath=NULL;
    char *savePath2=NULL;
    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);
    for(int i=0; i<256; i++)
    {
        b.palette[i].rgbBlue=i;
        b.palette[i].rgbGreen=i;
        b.palette[i].rgbRed=i;
    }

    bmp a;//ֱ��ͼ
    memcpy(&(a.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(a.biHeader),&biHeader,sizeof(biHeader));

    int bmp_zf_width = 256;
    int bmp_zf_height = 256;

    a.biHeader.biHeight = bmp_zf_height;
    a.biHeader.biWidth = bmp_zf_width;
    a.biHeader.biBitCount=24;
    int LineByte_zf=(bmp_zf_width*a.biHeader.biBitCount+31)/32*4;//��֤ÿһ���ֽ�����Ϊ4��������
    a.biHeader.biSizeImage=LineByte_zf*bmp_zf_height; //�ı�ͼ�����ݴ�С
    a.biHeader.biClrImportant=0;
    a.biHeader.biClrUsed=0;

    a.bfHeader.bfOffBits=54;
    a.bfHeader.bfSize=54+a.biHeader.biSizeImage; //�ı��ļ����ݴ�С;
    a.bfHeader.bfReserved1=0;
    a.bfHeader.bfReserved2=0;

    unsigned char *pBmpBuf_zf = (unsigned char *)malloc(LineByte_zf*bmp_zf_height*sizeof(unsigned char));  //��ȡδ���洢λͼ������Ҫ�Ŀռ�

    //������ֵ
    threshold=120;
    cout<<threshold<<endl;
    binaryzation(height,width,imageData,b,lineBytes,grayCount,threshold);
    //ֱ��ͼ
    zhifang(grayCount,threshold,LineByte_zf,pBmpBuf_zf);
    savePath="D:/ͼ����/test��/image/lena_given.bmp";
    savePath2="D:/ͼ����/test��/image/zhifang_given.bmp";
    Bwrite(savePath,b);
    Zwrite(savePath2,a,pBmpBuf_zf);

    int gray[imSize]= {0}; //��¼�Ҷ�ֵ
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            int data=imageData[lineBytes*i+j];
            gray[lineBytes*i+j] = imageData[lineBytes*i+j];
            grayCount[data]++;
        }
    }
    sort(gray,gray+imSize);
    int g=(int)imSize/2;
    threshold=gray[g];
    //������ֵ��,���Ϊ121
    threshold=Qthreshold(gray,threshold,imSize);
    cout<<threshold<<endl;
    //��ֵ��
    binaryzation(height,width,imageData,b,lineBytes,grayCount,threshold);
    //ֱ��ͼ
    zhifang(grayCount,threshold,LineByte_zf,pBmpBuf_zf);
    savePath="D:/ͼ����/test��/image/lena_iteration.bmp";
    savePath2="D:/ͼ����/test��/image/zhifang_iteration.bmp";
    Bwrite(savePath,b);
    Zwrite(savePath2,a,pBmpBuf_zf);

    int max=0;
    for(int i=0; i<256; i++)
    {
        int th=i;
        int otsu=0;
        otsu=Otsu(imageData,th,lineBytes,height,width);
        if(otsu>max)
        {
            max=otsu;
            threshold=i;
        }
    }
    cout<<threshold<<endl;
    //��ֵ��
    binaryzation(height,width,imageData,b,lineBytes,grayCount,threshold);
    //ֱ��ͼ
    zhifang(grayCount,threshold,LineByte_zf,pBmpBuf_zf);
    savePath="D:/ͼ����/test��/image/lena_otsu.bmp";
    savePath2="D:/ͼ����/test��/image/zhifang_otsu.bmp";
    Bwrite(savePath,b);
    Zwrite(savePath2,a,pBmpBuf_zf);

    free(imageData);
    free(b.imgData);
    return 0;
}

typedef struct
{
    int width0=0;
    int height0=0;
    int width1=0;
    int height1=0;
} DATA;

typedef struct tnode
{
    DATA data;
    tnode *one;
    tnode *two;
    tnode *three;
    tnode *four;
} tnode;

typedef struct
{
    tnode *root;
} tree;

tnode *create_node(DATA key)
{
    tnode* node = (tnode*)malloc(sizeof(tnode));

    node->data.width0 = key.width0;
    node->data.height0 = key.height0;
    node->data.width1 = key.width1;
    node->data.height1 = key.height1;

    node->one = node->two = node->three = node->four =NULL;
    return node;
}

void draw(DATA key,bmp b,int lineBytes)//����
{
    if(key.width0==key.width1)
    {
        for(int i=key.height0; i<=key.height1&&i<b.biHeader.biHeight; i++)
        {
            b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+key.width0*3]=0;
            b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+key.width0*3+1]=0;
            b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+key.width0*3+2]=0;
        }
    }
    if(key.height0==key.height1)
    {
        for(int i=key.width0; i<=key.width1&&i<b.biHeader.biWidth; i++)
        {

            b.imgData[lineBytes*(b.biHeader.biHeight-1-key.height0)+i*3]=0;
            b.imgData[lineBytes*(b.biHeader.biHeight-1-key.height0)+i*3+1]=0;
            b.imgData[lineBytes*(b.biHeader.biHeight-1-key.height0)+i*3+2]=0;
        }
    }
}

bool judge(DATA key,bmp b,int lineBytes)//�ж��Ƿ�Ӧ�÷ָ�
{
    int max=0;
    int min=255;
    for(int i=key.height0; i<=key.height1&&i<b.biHeader.biHeight; i++)
    {
        for(int j=key.width0; j<=key.width1&&j<b.biHeader.biWidth; j++)
        {
            if(b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+3*j]>max)
                max=b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+3*j];
            if(b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+3*j]<min)
            {
                min=b.imgData[lineBytes*(b.biHeader.biHeight-1-i)+3*j];
            }
        }
    }
    if(max-min>=100)
        return true;
    else
        return false;

}
int traversal(tnode *node,bmp b,int lineBytes)
{

    if(node==NULL) return 0;
    DATA key1;
    DATA key2;
    DATA key3;
    DATA key4;

    DATA dkey1;//����
    DATA dkey2;//����
    if (node->one == NULL)
    {
        if(judge(node->data,b,lineBytes)&&(node->data.width1-node->data.width0)*(node->data.height1-node->data.height0)>=100)
        {
            dkey1.height0=dkey1.height1=(node->data.height1+node->data.height0)/2;
            dkey1.width0=node->data.width0;
            dkey1.width1=node->data.width1;
            draw(dkey1,b,lineBytes);

            dkey2.width0=dkey2.width1=(node->data.width1+node->data.width0)/2;
            dkey2.height0=node->data.height0;
            dkey2.height1=node->data.height1;
            draw(dkey2,b,lineBytes);

            key1.width0=node->data.width0;
            key1.height0=node->data.height0;
            key1.width1=(node->data.width1+node->data.width0)/2-1;
            key1.height1=(node->data.height1+node->data.height0)/2-1;

            key2.width0=(node->data.width1+node->data.width0)/2+1;
            key2.height0=node->data.height0;
            key2.width1=node->data.width1;
            key2.height1=(node->data.height1+node->data.height0)/2-1;

            key3.width0=node->data.width0;
            key3.height0=(node->data.height1+node->data.height0)/2+1;
            key3.width1=(node->data.width1+node->data.width0)/2-1;
            key3.height1=node->data.height1;

            key4.width0=(node->data.width1+node->data.width0)/2+1;
            key4.height0=(node->data.height1+node->data.height0)/2+1;
            key4.width1=node->data.width1;
            key4.height1=node->data.height1;

            node->one= create_node(key1);
            node->two= create_node(key2);
            node->three = create_node(key3);
            node->four = create_node(key4);

        }
        else
            return 0;
    }

    traversal(node->one,b,lineBytes);
    traversal(node->two,b,lineBytes);
    traversal(node->three,b,lineBytes);
    traversal(node->four,b,lineBytes);

    return 0;
}

//�����ֵ������
void binaryzation(int height,int width,unsigned char*imageData,bmp b,int lineBytes,int threshold)
{
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(i>height*3/5||i<height/3)
            {
                b.imgData[lineBytes*i+j*3]=0;
                b.imgData[lineBytes*i+j*3+1]=0;
                b.imgData[lineBytes*i+j*3+2]=0;
            }
            else
            {
                int data=imageData[lineBytes*i+j*3];
                if(data>=threshold)
                {
                    b.imgData[lineBytes*i+j*3]=255;
                    b.imgData[lineBytes*i+j*3+1]=255;
                    b.imgData[lineBytes*i+j*3+2]=255;
                }
                else
                {
                    b.imgData[lineBytes*i+j*3]=0;
                    b.imgData[lineBytes*i+j*3+1]=0;
                    b.imgData[lineBytes*i+j*3+2]=0;
                }
            }

        }
    }
}
int exp6_1()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/ͼƬ1.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }

    int threshold=0;//��ֵ

    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);

    fclose(fp);

    char *savePath=NULL;

    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);

    threshold=225;

    //����
    char *savePathB="D:/ͼ����/test��/image/��ֵ��.bmp";
    binaryzation(height,width,imageData,b,lineBytes,threshold);
    Bwrite(savePathB,b);

    int T=20;
    float count=5;
    int c=1;
    while(c!=0)
    {
        c=0;
        for(int i=height-1; i>=0; i--)
        {
            for(int j=0; j<width; j++)
            {
                if(b.imgData[lineBytes*i+j*3+2]==255)//���������
                {
                    for(int g=i-1; g<=i+1; g++)
                    {
                        if(g<0||g>=height)//��ֹ�߶�Խ��
                            continue;

                        for(int k=j-1; k<=j+1; k++)
                        {
                            if(k<0||k>=width)//��ֹ���Խ��
                                continue;
                            if(b.imgData[lineBytes*g+k*3+2]==255)//����Ѿ������ӣ���������
                                continue;
                            if(abs(imageData[lineBytes*g+k*3]-imageData[lineBytes*i+j*3])<=T&&(255-imageData[lineBytes*g+k*3])<=T*count)//
                            {
                                b.imgData[lineBytes*g+k*3]=0;
                                b.imgData[lineBytes*g+k*3+1]=0;
                                b.imgData[lineBytes*g+k*3+2]=255;
                                c++;
                            }
                        }
                    }
                }
            }
        }
    }
    savePath="D:/ͼ����/test��/image/����.bmp";

    Bwrite(savePath,b);
    free(imageData);
    free(b.imgData);
    return 0;
}
int exp6_2()
{
    FILE *fp;
    char *bmpName="D:/ͼ����/test��/image/ͼƬ1.bmp";
    if((fp=fopen(bmpName,"rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }

    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);
    int imSize=biHeader.biSizeImage;
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;
    int lineBytes=(width*bitCount+31)/32*4;

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);

    fclose(fp);

    char *savePath=NULL;

    bmp b;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*imSize);
    memset(b.imgData,0,sizeof(unsigned char)*imSize);

    int threshold=225;


    //����
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes*i+j*3]=imageData[lineBytes*i+j*3];
            b.imgData[lineBytes*i+j*3+1]=imageData[lineBytes*i+j*3+1];
            b.imgData[lineBytes*i+j*3+2]=imageData[lineBytes*i+j*3+2];
        }
    }
    tnode *node=NULL;
    DATA key1;
    key1.height0=key1.width0=0;
    key1.width1=width;
    key1.height1=height;
    node=create_node(key1);
    traversal(node,b,lineBytes);
    savePath="D:/ͼ����/test��/image/����.bmp";
    Bwrite(savePath,b);
    free(imageData);
    free(b.imgData);
    return 0;
}

int exp7_1()
{

    FILE* fp ;
    if((fp=fopen("D:/ͼ����/test��/image/lena.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader fileHead;
    fread(&fileHead, sizeof(bitmapFileHeader), 1, fp);

    bitmapInfoHeader infoHead;
    fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);

    int width = infoHead.biWidth;
    int height = infoHead.biHeight;
    int biCount = infoHead.biBitCount;

    int lineByte = (biCount*width / 8 + 3) / 4 * 4;
    rgbQUAD* pColorTable;
    pColorTable = new rgbQUAD[256];
    fread(pColorTable, sizeof(rgbQUAD), 256, fp);

    unsigned char* pBmpBuf;
    pBmpBuf = new unsigned char[lineByte*height];
    fread(pBmpBuf, lineByte*height, 1, fp);
    fclose(fp);
    char *savePath;

    // ��ͼ

    unsigned char *pBmpBuf2;
    // ��ʼ��
    pBmpBuf2 = new unsigned char[lineByte*height];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            *(pBmpBuf2 + i*lineByte + j) = *(pBmpBuf + i*lineByte + j);
        }
    }


    int ul, uc, ur, dl, dc, dr;
    int lu, lc, ld, ru, rc, rd;
    int g;
    double hir, vec;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(j==0||i==0||i==height-1||j==width-1)
                *(pBmpBuf2+i*lineByte+j)=0;
        }
    }
    savePath="D:/ͼ����/test��/image/lena_sobel.bmp";
    //������
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            // ��ֱ�ݶ����ӣ����ˮƽ��Ե
            vec = 0;
            ul = *(pBmpBuf + (i + 1)*lineByte + (j - 1))*(-1);
            uc = *(pBmpBuf + (i + 1)*lineByte + j)*(-2);
            ur = *(pBmpBuf + (i + 1)*lineByte + (j + 1))*(-1);
            dl = *(pBmpBuf + (i - 1)*lineByte + (j - 1)) * 1;
            dc = *(pBmpBuf + (i - 1)*lineByte + j) * 2;
            dr = *(pBmpBuf + (i - 1)*lineByte + (j + 1)) * 1;
            vec = ul + uc + ur + dl + dc + dr;
            // ˮƽ�ݶ����ӣ���ⴹֱ��Ե
            hir = 0;
            lu = *(pBmpBuf + (i + 1)*lineByte + (j - 1))*(-1);
            lc = *(pBmpBuf + (i - 0)*lineByte + (j - 1))*(-2);
            ld = *(pBmpBuf + (i - 1)*lineByte + (j - 1))*(-1);
            ru = *(pBmpBuf + (i + 1)*lineByte + (j + 1)) * 1;
            rc = *(pBmpBuf + (i - 0)*lineByte + (j + 1)) * 2;
            rd = *(pBmpBuf + (i - 1)*lineByte + (j + 1)) * 1;
            hir = lu + lc + ld + ru + rc + rd;
            //��Ҷ�ֵ
            g=round(sqrt(hir*hir + vec*vec));
            //�ж��Ƿ������ֵ
            if(g>200)
                *(pBmpBuf2+i*lineByte+j) = 255;
            else
                *(pBmpBuf2+i*lineByte+j)=  0;
        }
    }
    FILE* fop = fopen(savePath, "wb");
    if(fop==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
    fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);
    fwrite(pBmpBuf2, lineByte*height, 1, fop);
    fclose(fop);

    printf("lena_sobelͼƬ�ɹ������\n");
    return 0;
}
int exp7_2()
{
    FILE* fp ;
    if((fp=fopen("D:/ͼ����/test��/image/lena.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader fileHead;
    fread(&fileHead, sizeof(bitmapFileHeader), 1, fp);

    bitmapInfoHeader infoHead;
    fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);
    int width = infoHead.biWidth;
    int height = infoHead.biHeight;
    int biCount = infoHead.biBitCount;

    int lineByte = (biCount*width / 8 + 3) / 4 * 4;
    rgbQUAD* pColorTable;
    pColorTable = new rgbQUAD[256];
    fread(pColorTable, sizeof(rgbQUAD), 256, fp);

    unsigned char* pBmpBuf;
    pBmpBuf = new unsigned char[lineByte*height];
    fread(pBmpBuf, lineByte*height, 1, fp);
    fclose(fp);
    char *savePath;

    // ��ͼ

    unsigned char *pBmpBuf2;
    // ��ʼ��
    pBmpBuf2 = new unsigned char[lineByte*height];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            *(pBmpBuf2 + i*lineByte + j) = *(pBmpBuf + i*lineByte + j);
        }
    }


    int ul, uc, ur, dl, dc, dr;
    int lu, lc, ld, ru, rc, rd;
    int g;
    double hir, vec;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(j==0||i==0||i==height-1||j==width-1)
                *(pBmpBuf2+i*lineByte+j)=0;
        }
    }
    savePath="D:/ͼ����/test��/image/lena_prewitt.bmp";
    //��³����
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            // ��ֱ�ݶ����ӣ����ˮƽ��Ե
            vec = 0;
            ul = *(pBmpBuf + (i + 1)*lineByte + (j - 1))*(-1);
            uc = *(pBmpBuf + (i + 1)*lineByte + j)*(-1);
            ur = *(pBmpBuf + (i + 1)*lineByte + (j + 1))*(-1);
            dl = *(pBmpBuf + (i - 1)*lineByte + (j - 1)) * 1;
            dc = *(pBmpBuf + (i - 1)*lineByte + j) * 1;
            dr = *(pBmpBuf + (i - 1)*lineByte + (j + 1)) * 1;
            vec = ul + uc + ur + dl + dc + dr;
            // ˮƽ�ݶ����ӣ���ⴹֱ��Ե
            hir = 0;
            lu = *(pBmpBuf + (i + 1)*lineByte + (j - 1))*(-1);
            lc = *(pBmpBuf + (i - 0)*lineByte + (j - 1))*(-1);
            ld = *(pBmpBuf + (i - 1)*lineByte + (j - 1))*(-1);
            ru = *(pBmpBuf + (i + 1)*lineByte + (j + 1)) * 1;
            rc = *(pBmpBuf + (i - 0)*lineByte + (j + 1)) * 1;
            rd = *(pBmpBuf + (i - 1)*lineByte + (j + 1)) * 1;
            hir = lu + lc + ld + ru + rc + rd;
            *(pBmpBuf2+i*lineByte+j) = round(sqrt(hir*hir + vec*vec));
            g=round(sqrt(hir*hir + vec*vec));
            if(g>200)
                *(pBmpBuf2+i*lineByte+j) = 255;
            else
                *(pBmpBuf2+i*lineByte+j)=  0;
        }
    }
    FILE* fop = fopen(savePath, "wb");
    if(fop==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
    fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);
    fwrite(pBmpBuf2, lineByte*height, 1, fop);
    fclose(fop);

    printf("lena_prewittͼƬ�ɹ������\n");
    return 0;
}
int exp7_3()
{
    FILE* fp ;
    if((fp=fopen("D:/ͼ����/test��/image/lena.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader fileHead;
    fread(&fileHead, sizeof(bitmapFileHeader), 1, fp);

    bitmapInfoHeader infoHead;
    fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);
    int width = infoHead.biWidth;
    int height = infoHead.biHeight;
    int biCount = infoHead.biBitCount;

    int lineByte = (biCount*width / 8 + 3) / 4 * 4;
    rgbQUAD* pColorTable;
    pColorTable = new rgbQUAD[256];
    fread(pColorTable, sizeof(rgbQUAD), 256, fp);

    unsigned char* pBmpBuf;
    pBmpBuf = new unsigned char[lineByte*height];
    fread(pBmpBuf, lineByte*height, 1, fp);
    fclose(fp);
    char *savePath;

    // ��ͼ

    unsigned char *pBmpBuf2;
    // ��ʼ��
    pBmpBuf2 = new unsigned char[lineByte*height];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            *(pBmpBuf2 + i*lineByte + j) = *(pBmpBuf + i*lineByte + j);
        }
    }

    int a1,a2,b1,b2,b3,b4,b5,b6,c1,c2,c3,c4,c5;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(j==0||i==0||i==height-1||j==width-1)
                *(pBmpBuf2+i*lineByte+j)=0;
        }
    }
    savePath="D:/ͼ����/test��/image/lena_LOG.bmp";
    //��˹-������˹
    for (int i = 1; i < height - 2; ++i)
    {
        for (int j = 1; j < width - 2; ++j)
        {

            int sum = 0;
            a1 = *(pBmpBuf + (i - 2)*lineByte + j)*1;
            a2 = *(pBmpBuf + (i + 2)*lineByte + j)*1;
            b1 = *(pBmpBuf + (i - 1)*lineByte + (j - 1))*1;
            b2 = *(pBmpBuf + (i - 1)*lineByte + (j - 0)) * 2;
            b3 = *(pBmpBuf + (i - 1)*lineByte + (j + 1)) * 1;
            b4 = *(pBmpBuf + (i + 1)*lineByte + (j - 1)) *1;
            b5 = *(pBmpBuf + (i + 1)*lineByte + (j - 0)) *2;
            b6 = *(pBmpBuf + (i + 1)*lineByte + (j + 1)) *1;
            c1 = *(pBmpBuf + (i - 0)*lineByte + (j - 2)) *1;
            c2 = *(pBmpBuf + (i - 0)*lineByte + (j - 1)) *2;
            c3 = *(pBmpBuf + (i - 0)*lineByte + (j + 0)) *(-16);
            c4 = *(pBmpBuf + (i - 0)*lineByte + (j + 1)) *2;
            c5 = *(pBmpBuf + (i - 0)*lineByte + (j + 2)) *1;
            sum = a1+a2+b1+b2+b3+b4+b5+b6+c1+c2+c3+c4+c5;
            if(round(sum)>200)
                *(pBmpBuf2+i*lineByte+j)=255;
            else
                *(pBmpBuf2+i*lineByte+j)= 0;
        }
    }
    FILE* fop = fopen(savePath, "wb");
    if(fop==NULL)
    {
        perror("can not open file!");
        return -2;
    }
    fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
    fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);
    fwrite(pBmpBuf2, lineByte*height, 1, fop);
    fclose(fop);

    printf("lena_LOGͼƬ�ɹ������\n");
    return 0;

}

int exp8()
{

	FILE* fp = fopen("D:/ͼ����/test��/image/test.bmp", "rb");
	if (fp == 0)
		return 0;
	bitmapFileHeader fileHead;
	fread(&fileHead,sizeof(bitmapFileHeader), 1, fp);

	bitmapInfoHeader infoHead;
	fread(&infoHead, sizeof(bitmapInfoHeader), 1, fp);
	int bmpsize= infoHead.biSizeImage;
	int width = infoHead.biWidth;
	int height = infoHead.biHeight;
	int biCount = infoHead.biBitCount;

	int lineByte = (biCount*width / 8 + 3) / 4 * 4;
	rgbQUAD* pColorTable;
	pColorTable = new rgbQUAD[256];
	fread(pColorTable, sizeof(rgbQUAD), 256, fp);

	unsigned char* pBmpBuf;
	pBmpBuf = new unsigned char[lineByte*height];
	fread(pBmpBuf, lineByte*height, 1, fp);
	fclose(fp);
	char *savePath;

	// ��ͼ
	unsigned char *pBmpBuf2;
	// ��ʼ��
	pBmpBuf2 = new unsigned char[lineByte*height];

	for (int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j){

        *(pBmpBuf2 + i*lineByte + j) = *(pBmpBuf + i*lineByte + j);
		}
	}

        savePath="D:/ͼ����/test��/image/out_line.bmp";
        FILE* fop = fopen(savePath, "wb");
        if (fop == 0)
            return 0;
        fwrite(&fileHead, sizeof(bitmapFileHeader), 1, fop);
        fwrite(&infoHead, sizeof(bitmapInfoHeader), 1, fop);
        fwrite(pColorTable, sizeof(rgbQUAD), 256, fop);


        int rho;
        int d=sqrt(width*width+height*height);
        int platoon_bit=bmpsize/height-width*(biCount/8);
        int* A=new int[d * 2 *180];
        int* theta=new int[180];
        stack<int> lineAIndexStack;
	    int lineAIndexLength = 0;
        for (int i = 0; i < 180; i++){
            theta[i] = i;
        }
        for (int i = 0; i < 180 * d * 2; i++){
            A[i] = 0;
        }
    for(int i=1;i<height;i++)
        for(int j=1;j<width;j++)
            if(*(pBmpBuf2+i*lineByte+j) == 255){
                    for(int k=0;k<180;k++){
                        rho=int(j*cos(theta[k] * 3.14 /180)+i*sin(theta[k] * 3.14/180))%d;
                        rho=rho+d;
                        A[rho *180 +k]++;
                    }
    }
    for(int i=0;i<d*2;i++){
        for(int j=0;j<180;j++){
            if (A[i * 180 + j] >= 80){
                lineAIndexStack.push(i * 180 + j);
				lineAIndexLength++;

            }
        }
    }

    int* lineAIndexArray = new int[lineAIndexLength];
	int i = 0;
	while (!lineAIndexStack.empty()) {
		lineAIndexArray[i] = lineAIndexStack.top();
		lineAIndexStack.pop();
		i++;
	}

	BYTE gbquadIndex;
	//cout<<lineAIndexLength;
	int k;
	int mh[10];
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			gbquadIndex = 0;
			//�ж�������ص��Ƿ���ĳ��ֱ������
			for (int i = 0; i < lineAIndexLength; i++)
			{
				int t = lineAIndexArray[i] % 180;//theta
				int r = lineAIndexArray[i] /180 - d;//rho
				if ((abs(column * cos(t * 3.14/180) + row * sin(t * 3.14/180) - r) <= 0.0001) )
					gbquadIndex = 255;
				if(t==90&&row==r)
                    gbquadIndex = 255;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, fop);
		}
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, fop);
	}
	cout << "ֱ�߼�⣨Hough���ɹ���" << endl;
	fclose(fop);

	printf("�ɹ������");
	return 0;
}

int exp9()
{
    FILE *fp;
    if((fp=fopen("D:/ͼ����/test��/image/1.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);//��λͼ��Ϣͷ
    int imSize=biHeader.biSizeImage;//�ļ���С
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;//��ɫλ��
    int lineBytes=(width*bitCount+31)/32*4;//���ֽ���

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    int *Flag=(int*)malloc(imSize*sizeof(int));
    memset(Flag,0,imSize*sizeof(int));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);//��λͼ����
    fclose(fp);

    bmp b;
    int c=0;
    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*b.biHeader.biSizeImage);
    memset(b.imgData,0,sizeof(unsigned char)*b.biHeader.biSizeImage);
    int equal_value[240][240]= {0};
    char*savePath="D:/ͼ����/test��/image/������.bmp";

    for(int i=height-1; i>=0; i--)
    {
        for(int j=0; j<width; j++)
        {
            if(imageData[lineBytes*i+j*3]!=0)
            {
                if(i==height&&j==0)//��һ�е�һ����
                {
                    Flag[lineBytes*i+j*3]=c++;
                }
                int count=0;
                int a[2]= {0};
                for(int n=i; n<=i+1; n++)
                {
                    if(n<0||n>=height)//��ֹԽ��
                        continue;
                    for(int m=j-1; m<=j+1; m++)
                    {
                        if(m<0||m>=width)//��ֹԽ��
                            continue;
                        if(n==i&&m>j-1)
                            continue;
                        if(Flag[lineBytes*n+m*3]!=0)
                        {
                            Flag[lineBytes*i+j*3]=Flag[lineBytes*n+m*3];
                            a[count]=Flag[lineBytes*n+m*3];
                            count++;
                        }

                    }
                }
                if(count==0)//������Ǳ���
                {
                    Flag[lineBytes*i+j*3]=c++;
                }
                if(a[0]!=a[1]&&a[1]!=0)
                {
                    equal_value[a[0]][a[1]]=equal_value[a[1]][a[0]]=1;//���ȼ۱�
                }
            }
        }
    }
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(Flag[lineBytes*i+j*3]!=0)
            {
                int f=1;
                while(f)
                {
                    f=0;
                    int n=Flag[lineBytes*i+j*3];
                    for(int m=0; m<n; m++)
                    {
                        if(equal_value[n][m]==1)
                        {
                            Flag[lineBytes*i+j*3]=m;
                             f=1;
                        }
                    }
                }
                b.imgData[lineBytes*i+j*3]=Flag[lineBytes*i+j*3]*10;
                b.imgData[lineBytes*i+j*3+1]=Flag[lineBytes*i+j*3]*10;
                b.imgData[lineBytes*i+j*3+2]=Flag[lineBytes*i+j*3]*10;
                int k=Flag[lineBytes*i+j*3]%3;
                b.imgData[lineBytes*i+j*3+k]=255;
            }
        }
    }

    write(savePath,b);

    free(imageData);
    free(b.imgData);
    return 0;
}

int exp10()
{
    FILE *fp;
    if((fp=fopen("D:/ͼ����/test��/imagE/ͼƬ3.bmp","rb"))==NULL)
    {
        perror("can not open file!");
        return -1;
    }
    bitmapFileHeader bfHeader;
    fread(&bfHeader,14,1,fp);//��λͼ�ļ�ͷ
    bitmapInfoHeader biHeader;
    fread(&biHeader,40,1,fp);//��λͼ��Ϣͷ
    int imSize=biHeader.biSizeImage;//�ļ���С
    int width=biHeader.biWidth;
    int height=biHeader.biHeight;
    int bitCount=biHeader.biBitCount;//��ɫλ��
    int lineBytes=(width*bitCount+31)/32*4;//���ֽ���

    fseek(fp,bfHeader.bfOffBits,SEEK_SET);
    unsigned char*imageData=(unsigned char*)malloc(imSize*sizeof(unsigned char));
    int *Flag=(int*)malloc(imSize*sizeof(int));
    memset(Flag,0,imSize*sizeof(int));
    fread(imageData,imSize*sizeof(unsigned char),1,fp);//��λͼ����
    fclose(fp);

    bmp b;

    memcpy(&(b.bfHeader),&bfHeader,sizeof(bfHeader));
    memcpy(&(b.biHeader),&biHeader,sizeof(biHeader));
    b.imgData=(unsigned char*)malloc(sizeof(unsigned char)*b.biHeader.biSizeImage);
    memset(b.imgData,0,sizeof(unsigned char)*b.biHeader.biSizeImage);
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            b.imgData[lineBytes*i+j*3]=imageData[lineBytes*i+j*3];
            b.imgData[lineBytes*i+j*3+1]=imageData[lineBytes*i+j*3+1];
            b.imgData[lineBytes*i+j*3+2]=imageData[lineBytes*i+j*3+2];
        }
    }

    char*savePath="D:/ͼ����/test��/image/������ȡ.bmp";

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(b.imgData[lineBytes*i+j*3]==0)
            {
                int count=0;
                for(int n=i-1; n<=i+1; n++)
                {
                    if(n<0||n>=height)//��ֹԽ��
                    {
                        count++;
                        continue;
                    }
                    for(int m=j-1; m<=j+1; m++)
                    {
                        if(m<0||m>=width)//��ֹԽ��
                        {
                            count++;
                            continue;
                        }
                        if(b.imgData[lineBytes*n+m*3]==0||Flag[lineBytes*n+m*3]==1)
                        {
                            count++;
                        }
                    }
                }
                if(count==9)
                {
                    Flag[lineBytes*i+j*3]=1;
                    b.imgData[lineBytes*i+j*3]=255;
                    b.imgData[lineBytes*i+j*3+1]=255;
                    b.imgData[lineBytes*i+j*3+2]=255;
                }
            }
        }
    }

    write(savePath,b);

    free(imageData);
    free(b.imgData);
    return 0;
}

int main()
{

    cout<<"********** �˵� ************"<<endl;
    cout<<"****** 0.  �����˳� ********"<<endl;
    cout<<"****** 1.  BMP�ļ����� *****"<<endl;
    cout<<"****** 2.  ֱ��ͼ���� ******"<<endl;
    cout<<"****** 3.  �ռ����˲� ******"<<endl;
    cout<<"****** 4.  ͼ��任 ********"<<endl;
    cout<<"****** 5.  ��ֵ�ָ� ********"<<endl;
    cout<<"****** 6.  ��������ķָ� **"<<endl;
    cout<<"****** 7.  ��Ե��� ********"<<endl;
    cout<<"****** 8.  Hough�任 *******"<<endl;
    cout<<"****** 9.  ������ ********"<<endl;
    cout<<"****** 10. ������ȡ ********"<<endl;
    cout<<"****** 11. ������ **********"<<endl;
    cout<<"��ѡ��"<<endl;
    int option;
    cin>>option;
    while(option!=0)
    {
        switch(option)
        {
        case 0:
            cout<<"�����˳�"<<endl;
            break;
        case 1:
            exp1();
            break;
        case 2:
            exp2();
            break;
        case 3:
            exp3_1();
            exp3_2();
            break;
        case 4:
            exp4_1();
            exp4_2();
            exp4_3();
            exp4_4();
            break;
        case 5:
            exp5();
            break;
        case 6:
            exp6_1();
            exp6_2();
            break;
        case 7:
            exp7_1();
            exp7_2();
            exp7_3();
            break;
        case 8:
            exp8();
            break;
        case 9:
            exp9();
            break;
        case 10:
            exp10();
            break;
        case 11:
            exp1();
            exp2();
            exp3_1();
            exp3_2();
            exp4_1();
            exp4_2();
            exp4_3();
            exp4_4();
            exp5();
            exp6_1();
            exp6_2();
            exp7_1();
            exp7_2();
            exp7_3();
            exp8();
            exp9();
            exp10();
            break;
        default:
            cout<<"��ѡ����ڣ����������룡"<<endl;
        }
        cout<<"\n��ѡ��"<<endl;
        cin>>option;
    }
}

