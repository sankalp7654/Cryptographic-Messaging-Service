// Developed by Nishant Sharma
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define clear() printf("\033[H\033[J");			//For clearing the screen
typedef unsigned char uch;				
uch key[]="1010101010101010";				//Symmetric key used in AES.

// Structure for storing user details.

typedef struct userDetails
{
	char name[40];
	char userName[10];
	char password[15];
	char gender[10];
}userData;

				// All the Function Prototypes //
/*************************************************************************************************/
void mainMenu(userData u[], int *count);
void userDashboard(userData u[], int *count);
void displayMessage(userData u[], int *count);
void sendMessage(userData u[], int *db_size);
void userRegister(userData u[], int *count);
void userLogin(userData u[], int *db_size);
void wrongCredentials(userData u[], int *db_size);
void actualMessage(char usr_name[]);
void removeFiles(userData u[], int *count);

void addKey(uch mat[4][4]);
void invaddKey(uch mat[4][4]);
void byteSub(uch mat[4][4]);
void invbyteSub(uch mat[4][4]);
void shiftRow(uch mat[4][4]);
void invShiftRow(uch mat[4][4]);
void encrypt(uch mat[4][4]);
void decrypt(uch mat[4][4]);
void performAesEncryption(uch message[]);
void performAesDecryption(uch message[]);
void mixCol(uch mat[4][4]);
void invMixCol(uch mat[4][4]);
/*************************************************************************************************/

//S-box table is stored in the form of string.
uch sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,   //0
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,   //1
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,   //2
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,   //3
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,   //4
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,   //5
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,   //6
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,   //7
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,   //8
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,   //9
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,   //A
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,   //B
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,   //C
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,   //D
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,   //E
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

//Reverse S-box table is stored in the form of string.
uch rsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

uch mul9[256] = {
  0x00, 0x09, 0x12,0x1b,0x24,0x2d,0x36,0x3f,0x48,0x41,0x5a,0x53,0x6c,0x65,0x7e,0x77,
  0x90, 0x99, 0x82,0x8b,0xb4,0xbd,0xa6,0xaf,0xd8,0xd1,0xca,0xc3,0xfc,0xf5,0xee,0xe7,
  0x3b, 0x32, 0x29,0x20,0x1f,0x16,0x0d,0x04,0x73,0x7a,0x61,0x68,0x57,0x5e,0x45,0x4c,
  0xab, 0xa2, 0xb9,0xb0,0x8f,0x86,0x9d,0x94,0xe3,0xea,0xf1,0xf8,0xc7,0xce,0xd5,0xdc,
  0x76, 0x7f, 0x64,0x6d,0x52,0x5b,0x40,0x49,0x3e,0x37,0x2c,0x25,0x1a,0x13,0x08,0x01,
  0xe6, 0xef, 0xf4,0xfd,0xc2,0xcb,0xd0,0xd9,0xae,0xa7,0xbc,0xb5,0x8a,0x83,0x98,0x91,
  0x4d, 0x44, 0x5f,0x56,0x69,0x60,0x7b,0x72,0x05,0x0c,0x17,0x1e,0x21,0x28,0x33,0x3a,
  0xdd, 0xd4, 0xcf,0xc6,0xf9,0xf0,0xeb,0xe2,0x95,0x9c,0x87,0x8e,0xb1,0xb8,0xa3,0xaa,
  0xec, 0xe5, 0xfe,0xf7,0xc8,0xc1,0xda,0xd3,0xa4,0xad,0xb6,0xbf,0x80,0x89,0x92,0x9b,
  0x7c, 0x75, 0x6e,0x67,0x58,0x51,0x4a,0x43,0x34,0x3d,0x26,0x2f,0x10,0x19,0x02,0x0b,
  0xd7, 0xde, 0xc5,0xcc,0xf3,0xfa,0xe1,0xe8,0x9f,0x96,0x8d,0x84,0xbb,0xb2,0xa9,0xa0,
  0x47, 0x4e, 0x55,0x5c,0x63,0x6a,0x71,0x78,0x0f,0x06,0x1d,0x14,0x2b,0x22,0x39,0x30,
  0x9a, 0x93, 0x88,0x81,0xbe,0xb7,0xac,0xa5,0xd2,0xdb,0xc0,0xc9,0xf6,0xff,0xe4,0xed,
  0x0a, 0x03, 0x18,0x11,0x2e,0x27,0x3c,0x35,0x42,0x4b,0x50,0x59,0x66,0x6f,0x74,0x7d,
  0xa1, 0xa8, 0xb3,0xba,0x85,0x8c,0x97,0x9e,0xe9,0xe0,0xfb,0xf2,0xcd,0xc4,0xdf,0xd6,
  0x31, 0x38, 0x23,0x2a,0x15,0x1c,0x07,0x0e,0x79,0x70,0x6b,0x62,0x5d,0x54,0x4f,0x46 };

uch mul11[256] = {
  0x00,0x0b,0x16,0x1d,0x2c,0x27,0x3a,0x31,0x58,0x53,0x4e,0x45,0x74,0x7f,0x62,0x69,
  0xb0,0xbb,0xa6,0xad,0x9c,0x97,0x8a,0x81,0xe8,0xe3,0xfe,0xf5,0xc4,0xcf,0xd2,0xd9,
  0x7b,0x70,0x6d,0x66,0x57,0x5c,0x41,0x4a,0x23,0x28,0x35,0x3e,0x0f,0x04,0x19,0x12,
  0xcb,0xc0,0xdd,0xd6,0xe7,0xec,0xf1,0xfa,0x93,0x98,0x85,0x8e,0xbf,0xb4,0xa9,0xa2,
  0xf6,0xfd,0xe0,0xeb,0xda,0xd1,0xcc,0xc7,0xae,0xa5,0xb8,0xb3,0x82,0x89,0x94,0x9f,
  0x46,0x4d,0x50,0x5b,0x6a,0x61,0x7c,0x77,0x1e,0x15,0x08,0x03,0x32,0x39,0x24,0x2f,
  0x8d,0x86,0x9b,0x90,0xa1,0xaa,0xb7,0xbc,0xd5,0xde,0xc3,0xc8,0xf9,0xf2,0xef,0xe4,
  0x3d,0x36,0x2b,0x20,0x11,0x1a,0x07,0x0c,0x65,0x6e,0x73,0x78,0x49,0x42,0x5f,0x54,
  0xf7,0xfc,0xe1,0xea,0xdb,0xd0,0xcd,0xc6,0xaf,0xa4,0xb9,0xb2,0x83,0x88,0x95,0x9e,
  0x47,0x4c,0x51,0x5a,0x6b,0x60,0x7d,0x76,0x1f,0x14,0x09,0x02,0x33,0x38,0x25,0x2e,
  0x8c,0x87,0x9a,0x91,0xa0,0xab,0xb6,0xbd,0xd4,0xdf,0xc2,0xc9,0xf8,0xf3,0xee,0xe5,
  0x3c,0x37,0x2a,0x21,0x10,0x1b,0x06,0x0d,0x64,0x6f,0x72,0x79,0x48,0x43,0x5e,0x55,
  0x01,0x0a,0x17,0x1c,0x2d,0x26,0x3b,0x30,0x59,0x52,0x4f,0x44,0x75,0x7e,0x63,0x68,
  0xb1,0xba,0xa7,0xac,0x9d,0x96,0x8b,0x80,0xe9,0xe2,0xff,0xf4,0xc5,0xce,0xd3,0xd8,
  0x7a,0x71,0x6c,0x67,0x56,0x5d,0x40,0x4b,0x22,0x29,0x34,0x3f,0x0e,0x05,0x18,0x13,
  0xca,0xc1,0xdc,0xd7,0xe6,0xed,0xf0,0xfb,0x92,0x99,0x84,0x8f,0xbe,0xb5,0xa8,0xa3 };

uch mul13[256] = {
  0x00,0x0d,0x1a,0x17,0x34,0x39,0x2e,0x23,0x68,0x65,0x72,0x7f,0x5c,0x51,0x46,0x4b,
  0xd0,0xdd,0xca,0xc7,0xe4,0xe9,0xfe,0xf3,0xb8,0xb5,0xa2,0xaf,0x8c,0x81,0x96,0x9b,
  0xbb,0xb6,0xa1,0xac,0x8f,0x82,0x95,0x98,0xd3,0xde,0xc9,0xc4,0xe7,0xea,0xfd,0xf0,
  0x6b,0x66,0x71,0x7c,0x5f,0x52,0x45,0x48,0x03,0x0e,0x19,0x14,0x37,0x3a,0x2d,0x20,
  0x6d,0x60,0x77,0x7a,0x59,0x54,0x43,0x4e,0x05,0x08,0x1f,0x12,0x31,0x3c,0x2b,0x26,
  0xbd,0xb0,0xa7,0xaa,0x89,0x84,0x93,0x9e,0xd5,0xd8,0xcf,0xc2,0xe1,0xec,0xfb,0xf6,
  0xd6,0xdb,0xcc,0xc1,0xe2,0xef,0xf8,0xf5,0xbe,0xb3,0xa4,0xa9,0x8a,0x87,0x90,0x9d,
  0x06,0x0b,0x1c,0x11,0x32,0x3f,0x28,0x25,0x6e,0x63,0x74,0x79,0x5a,0x57,0x40,0x4d,
  0xda,0xd7,0xc0,0xcd,0xee,0xe3,0xf4,0xf9,0xb2,0xbf,0xa8,0xa5,0x86,0x8b,0x9c,0x91,
  0x0a,0x07,0x10,0x1d,0x3e,0x33,0x24,0x29,0x62,0x6f,0x78,0x75,0x56,0x5b,0x4c,0x41,
  0x61,0x6c,0x7b,0x76,0x55,0x58,0x4f,0x42,0x09,0x04,0x13,0x1e,0x3d,0x30,0x27,0x2a,
  0xb1,0xbc,0xab,0xa6,0x85,0x88,0x9f,0x92,0xd9,0xd4,0xc3,0xce,0xed,0xe0,0xf7,0xfa,
  0xb7,0xba,0xad,0xa0,0x83,0x8e,0x99,0x94,0xdf,0xd2,0xc5,0xc8,0xeb,0xe6,0xf1,0xfc,
  0x67,0x6a,0x7d,0x70,0x53,0x5e,0x49,0x44,0x0f,0x02,0x15,0x18,0x3b,0x36,0x21,0x2c,
  0x0c,0x01,0x16,0x1b,0x38,0x35,0x22,0x2f,0x64,0x69,0x7e,0x73,0x50,0x5d,0x4a,0x47,
  0xdc,0xd1,0xc6,0xcb,0xe8,0xe5,0xf2,0xff,0xb4,0xb9,0xae,0xa3,0x80,0x8d,0x9a,0x97 };

uch mul14[256] = {
  0x00,0x0e,0x1c,0x12,0x38,0x36,0x24,0x2a,0x70,0x7e,0x6c,0x62,0x48,0x46,0x54,0x5a,
  0xe0,0xee,0xfc,0xf2,0xd8,0xd6,0xc4,0xca,0x90,0x9e,0x8c,0x82,0xa8,0xa6,0xb4,0xba,
  0xdb,0xd5,0xc7,0xc9,0xe3,0xed,0xff,0xf1,0xab,0xa5,0xb7,0xb9,0x93,0x9d,0x8f,0x81,
  0x3b,0x35,0x27,0x29,0x03,0x0d,0x1f,0x11,0x4b,0x45,0x57,0x59,0x73,0x7d,0x6f,0x61,
  0xad,0xa3,0xb1,0xbf,0x95,0x9b,0x89,0x87,0xdd,0xd3,0xc1,0xcf,0xe5,0xeb,0xf9,0xf7,
  0x4d,0x43,0x51,0x5f,0x75,0x7b,0x69,0x67,0x3d,0x33,0x21,0x2f,0x05,0x0b,0x19,0x17,
  0x76,0x78,0x6a,0x64,0x4e,0x40,0x52,0x5c,0x06,0x08,0x1a,0x14,0x3e,0x30,0x22,0x2c,
  0x96,0x98,0x8a,0x84,0xae,0xa0,0xb2,0xbc,0xe6,0xe8,0xfa,0xf4,0xde,0xd0,0xc2,0xcc,
  0x41,0x4f,0x5d,0x53,0x79,0x77,0x65,0x6b,0x31,0x3f,0x2d,0x23,0x09,0x07,0x15,0x1b,
  0xa1,0xaf,0xbd,0xb3,0x99,0x97,0x85,0x8b,0xd1,0xdf,0xcd,0xc3,0xe9,0xe7,0xf5,0xfb,
  0x9a,0x94,0x86,0x88,0xa2,0xac,0xbe,0xb0,0xea,0xe4,0xf6,0xf8,0xd2,0xdc,0xce,0xc0,
  0x7a,0x74,0x66,0x68,0x42,0x4c,0x5e,0x50,0x0a,0x04,0x16,0x18,0x32,0x3c,0x2e,0x20,
  0xec,0xe2,0xf0,0xfe,0xd4,0xda,0xc8,0xc6,0x9c,0x92,0x80,0x8e,0xa4,0xaa,0xb8,0xb6,
  0x0c,0x02,0x10,0x1e,0x34,0x3a,0x28,0x26,0x7c,0x72,0x60,0x6e,0x44,0x4a,0x58,0x56,
  0x37,0x39,0x2b,0x25,0x0f,0x01,0x13,0x1d,0x47,0x49,0x5b,0x55,0x7f,0x71,0x63,0x6d,
  0xd7,0xd9,0xcb,0xc5,0xef,0xe1,0xf3,0xfd,0xa7,0xa9,0xbb,0xb5,0x9f,0x91,0x83,0x8d };

uch mul2[256] = {
0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5 };

uch mul3[256] = {
0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a };




char loggedUserName[10]="";			        // Gloabal variable that stores logged users's Username.

int main()
{
	userData u[20];			                // Array of userDetails structure, used to store the credentials of each User.
	int count = 0;				  	// count variable is used to store the no. of users registered.
	clear();
	mainMenu(u, &count);
}


// This is the main Menu screen, shown to the user at the start of the program.

void mainMenu(userData u[], int *count)
{
    int option;
    printf("\n Welcome to the Messaging Application!\n");
    printf(" ------------------------------------\n");
    printf(" Press any menu option number\n");
    printf(" ----------------------------\n");
    printf(" 1. Login\n");
    printf(" 2. Register\n");
    printf(" 3. Exit\n\n ");

    scanf("%d", &option);
    clear();

    switch(option)
    {
        case 1: userLogin(u, count);                           // Call to the function where User can Login.
        	break;

        case 2: userRegister(u, count);         	       // Call to the function where User can Register.
                break;

        case 3: removeFiles(u, count);				
		exit(0);                                       // Exit the program.

        default: clear();
		 printf("\n Invalid option selected. Try again \n");
                 mainMenu(u, count);  		               // Loop back to the Main Screen, so that user can again make his selection.
    }
}


// This function allows the Users to Login with their Username and Password.

void userLogin(userData u[], int *db_size)     			 // Here, second parameter is the no. of users who have already registered.
{
    int check_username, check_password, input, i, usr_found = 0;
    char usr_name[10], usr_pass[15];

    printf("\n (Press 1 to continue)\n");
    printf(" (Press 2 to go back to the previous menu)\n\n ");
    scanf("%d",&input);
    clear();

    if(input == 1)
    {

        printf("\n Please enter your Login details\n");
        printf(" -------------------------------\n");
        printf("\n Enter Username\n ");
        scanf("%s", usr_name);
        printf(" Enter Password\n ");
        scanf("%s", usr_pass);
        clear();

        // Check for the User details in the database and authorise to login.

        for(i=0;i<(*db_size);i++)
        {
        	check_username = strcmp(usr_name, u[i].userName);
	    	check_password = strcmp(usr_pass, u[i].password);
	    	if(check_username == 0 && check_password == 0)
	    	{
				usr_found = 1;			// usr_found is set to 1, if user details are found in the database.
				break;
	    	}
	}
        if(usr_found == 1)
	{
		strcpy(loggedUserName, usr_name);		// Copy logged User's username to global variable loggedUserName.
                userDashboard(u, db_size);           		// Take the User to his/her Dashboard.
	}
                
        else
                wrongCredentials(u, db_size);          		// To notify the users about wrong credentials and let them try again.
    }
    
    else if(input == 2)
        mainMenu(u, db_size);           			  // To take the user back to the Main Menu.
        
    else
    {
        printf("\n Invalid option selected. Please try again\n");
        userLogin(u, db_size);            			  // Let the user try again and select a valid option.
    }
}

// Function to notify the Users about their wrong credentials and let them try again.

void wrongCredentials(userData u[], int *db_size)
{
    printf("\n Invalid Username or Password\n");
    printf(" Please check the details and try again.\n");
    userLogin(u, db_size);
}


// This function allows the Users to Register their accounts.

void userRegister (userData u[], int *count)
{
	char confirm_password[15];			// Auxiliary variable to check if the passwords entered are consistent.
	int input, i;
	printf("\n (Press 1 to continue)\n");
    	printf(" (Press 2 to go back to the previous menu)\n\n ");
    	scanf("%d",&input);
        clear();
    
	if(input == 1)
	{
		printf("\n Please enter the following details to register a new account.\n");
       	        printf(" -------------------------------------------------------------\n\n");
		printf(" Enter Your Name \n ");
		scanf(" %[^\n]s", u[*count].name);
		printf(" Enter Your Gender \n ");
		scanf(" %s", u[*count].gender);
		
		tryUsername:	printf(" Enter a suitable Username \n ");
		
				scanf(" %s", u[*count].userName);
					
				for(i=0 ; i<(*count) ; i++)
				{
					if(strcmp(u[*count].userName, u[i].userName) == 0)	//To check if the username entered is unique.
					{
						printf("\n Username already exists. Please try something else.\n");
						goto tryUsername;	//Will jump to tryUsername and again ask user to enter Username.
					}
				}
		
		tryPassword:	printf(" Enter a strong Password \n ");
		
				scanf(" %s", u[*count].password);
				printf(" Confirm your Password \n ");
				scanf(" %s", confirm_password);
		
				if(strcmp(u[*count].password, confirm_password) != 0)		// To check if the passwords entered are same.
				{
					printf(" \nPasswords didn't match. Please try again.\n");
					goto tryPassword;		//Will jump to tryPassword and again ask user to enter password.
				}
	
				(*count)++;					 // Incremented, to update the no. of users registered.
				clear();		
				strcpy(loggedUserName, u[*count].userName);//Copy logged User's username to global variable loggedUserName.	
				userDashboard(u, count);				// Take the User to the Dashboard.
	}
	
	else if(input == 2)
        mainMenu(u, count);           			  			// To take the user back to the Main Menu.
        
    else
    {
        printf("\n Invalid option selected. Please try again\n");
        userRegister(u, count);            			  	// Let the user try again and select a valid option.
    }
}


// This is where the User is always logged on to, in the beginning.

void userDashboard(userData u[], int *count)
{
	int choice;
	printf("\n Welcome to your Dashboard!\n");
	printf(" --------------------------\n");
	
	tryDashB: printf(" 1. Send Message \n 2. View Message \n 3. Logout \n 4. Exit \n\n ");
	
	scanf("%d", &choice );
	clear();
	switch(choice)
	{
		case 1:	sendMessage(u, count);			 //Call the function which allow User to send message.
			break;

		case 2: displayMessage(u, count);		 //Call the function which allow user to view most recent message.
			break;

		case 3: mainMenu(u, count);			 //Take the user to Main Menu.
			break;

		case 4: removeFiles(u, count);
			exit(0);

		default: printf(" Invalid option selected. Please try again. \n");
			 userDashboard(u, count);		    //Let the user try again and select a valid option.
	}
}

// This function handles the Username of the person, whom the user wants to send a message

void sendMessage(userData u[], int *db_size)
{
    int input, check, i;
    char usr_name[10];

    printf("\n (Press 1 to continue)\n");
    printf(" (Press 2 to go back to previous menu)\n\n ");
    scanf("%d",&input);
    clear();

    if(input == 1)
    {
        printf("\n Enter the Username to send message\n ");
        scanf("%s",usr_name);
	for(i=0;i<(*db_size);i++)
	{
            check = strcmp(usr_name, u[i].userName);   		// Compare the username with the database and check for validity.
	    if(check == 0)
		break;
	}
        if(check == 0)
        {
            actualMessage(usr_name);        			// Call to the function which handles the message to be sent.
        }
        else
        {
            printf(" Username not found in the database. Please try again\n");
            sendMessage(u, db_size);    			// Call back, to allow the user to enter a valid username of the recipient.
        }
    }
    else if(input == 2)
        userDashboard(u, db_size);       			// Move the user back to the dashboard.
    else
    {
        printf(" Invalid option selected. Please try again\n");
        sendMessage(u, db_size);       				// Allow the user to input a valid option.
    }
    userDashboard(u, db_size);

}

// This is a helper function to the sendMessage function which stores the actual Message to be sent in a file.
void actualMessage(char usr_name[])
{
    FILE *fp;
    int ch;
    uch msg[400];
    char msg_filename[15];
    strcpy(msg_filename,usr_name);					
    strcat(msg_filename,".txt");					// Append the filename with '.txt' to make it a proper file name.
    fp = fopen(msg_filename,"w");
   
    
    while ((ch = getchar()) != '\n' && ch != EOF);                      // To clear input buffer.
    
    printf("\n (Please enter the message for %s and then press (Tab -> Enter) to send it)\n ", usr_name);
    printf(" MESSAGE:\n\n");
    gets(msg);
    //scanf("%[^\t]s",msg);
    performAesEncryption(msg);						//Function will encrypt the message.
    //fprintf(fp,"%s",msg);						//Encrypted message is stored in a file.
    fputs(msg, fp);
    fclose(fp);
    clear();
}

//This function displays the most recent message recieved by the logged user.

void displayMessage(userData u[], int *count)
{
	FILE *fp;
	char msg_filename[15];
	int choice;
	uch message[400];

	strcpy(msg_filename,loggedUserName);				// Copy the logged user's Username to variable msg_filename.
	strcat(msg_filename,".txt"); 					// Append the filename with '.txt' to make it a proper file name.
	fp = fopen(msg_filename,"r");

	if( fp == NULL )
		printf(" No Messages for you! \n\n");
	else
	{
		//fscanf(fp,"%[^\n]s",message);			//Encrypted message is stored in message string.
		fgets(message, 400, fp);
		performAesDecryption(message);			//Function will decrypt the encrypted message.
		printf("%s\n",message);
		printf("\n");
		fclose(fp);
	}

	while(1)						//Loop will terminate when user will press 1.
	{

		printf(" (Press 1 to go back to the previous menu.) \n ");
		scanf("%d",&choice);
		clear();
		if(choice == 1)
		{
			userDashboard(u, count);
			break;
		}
		else
			printf(" Invalid input try again \n");
	}
}

//This function will remove all files containing messages send by different user.

void removeFiles(userData u[], int *count)
{
	char filename[15];
	int i;
	for(i=0 ; i<(*count) ; i++)
	{ 
		strcpy(filename,u[i].userName);
		strcat(filename,".txt");
		remove(filename);
	}
}

/*This function will convert original message in various 4X4 matrix (16 byte).
**Every matrix is encrypted and then encryped form will be restored in message string.*/
void performAesEncryption(uch message[])
{
	int i,j,k,m;
	uch mat[4][4];

	for(k=0;k<strlen(message);)
	{
		m=k;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(k<strlen(message))
					mat[j][i]=message[k];
				else
					mat[j][i]=' ';
				k++;
			}
		}
		encrypt(mat);
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				message[m]=mat[j][i];
				m++;
			}
		}
	}
	message[m]='\0';
	
}

/*This function will convert encrypted message in various 4X4 matrix (16 byte).
**Every matrix is decrypted and then decryped form will be restored in message string.*/
void performAesDecryption(uch message[])
{
	int i,j,k,m;
	uch mat[4][4];
	
	for(k=0;k<strlen(message);)
	{
		m=k;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				mat[j][i]=message[k];
				k++;
			}
		}
		decrypt(mat);
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				message[m]=mat[j][i];
				m++;
			}
		}
	}
	message[m]='\0';
}

//This function will manage operations to be performed in each round of AES Encryption.
void encrypt(uch mat[4][4])
{
	int i;

	addKey(mat);
	//round 1 to 9
	for(i=1;i<=9;i++)
	{
		//byteSub(mat);
		shiftRow(mat);
		mixCol(mat);
		addKey(mat);
	}
	
	//round 10
	//byteSub(mat);
	shiftRow(mat);
	addKey(mat);
}

//This function will manage operations to be performed in each round of AES Decryption.
void decrypt(uch mat[4][4])
{
	int i;
	
	//round 10
	invaddKey(mat);
	invShiftRow(mat);
	//invbyteSub(mat);
	
	//round 9 to 1
	for(i=9;i>=1;i--)
	{
		invaddKey(mat);
		invMixCol(mat);
		invShiftRow(mat);
		//invbyteSub(mat);
	}
	invaddKey(mat);
}

//This function will perform XOR operation for each byte of the matrix mat with corresponding byte of key.
void addKey(uch mat[4][4])
{
	int i,j,k=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			mat[j][i] = mat[j][i] ^ key[k];
			k++;
		}

	}

}

//This function will also perform XOR operation for each byte of the matrix mat with corresponding byte of key.
void invaddKey(uch mat[4][4])
{
	int i,j,k=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			mat[j][i] = mat[j][i] ^ key[k];
			k++;
		}

	}
}

//This function will substitute each byte by looking up to S-box table.
void byteSub(uch mat[4][4])
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
			mat[j][i]=sbox[mat[j][i]];
	}
}

//This function will substitute each byte by looking up to RS-box table.
void invbyteSub(uch mat[4][4])
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
			mat[j][i]=rsbox[mat[j][i]];
	}
}

//This function will left shift rows of matrix mat in a circular pattern.
void shiftRow(uch mat[4][4])
{
	uch temp;

	//2nd row is left shifted once.
	temp=mat[1][0];
	mat[1][0]=mat[1][1];
	mat[1][1]=mat[1][2];
	mat[1][2]=mat[1][3];
	mat[1][3]=temp;

	//3rd row is left shifted twice.
	temp=mat[2][0];
	mat[2][0]=mat[2][1];
	mat[2][1]=mat[2][2];
	mat[2][2]=mat[2][3];
	mat[2][3]=temp;

	temp=mat[2][0];
	mat[2][0]=mat[2][1];
	mat[2][1]=mat[2][2];
	mat[2][2]=mat[2][3];
	mat[2][3]=temp;
	
	//4th row is left shifted thrice.
	temp=mat[3][0];
	mat[3][0]=mat[3][1];
	mat[3][1]=mat[3][2];
	mat[3][2]=mat[3][3];
	mat[3][3]=temp;

	temp=mat[3][0];
	mat[3][0]=mat[3][1];
	mat[3][1]=mat[3][2];
	mat[3][2]=mat[3][3];
	mat[3][3]=temp;
	
	temp=mat[3][0];
	mat[3][0]=mat[3][1];
	mat[3][1]=mat[3][2];
	mat[3][2]=mat[3][3];
	mat[3][3]=temp;		
}

//This fucntion will right shift rows of matrix mat in circular manner.
void invShiftRow(uch mat[4][4])
{
	uch temp;
	
	//2nd row is right shifted once.
	temp=mat[1][3];
	mat[1][3]=mat[1][2];
	mat[1][2]=mat[1][1];
	mat[1][1]=mat[1][0];
	mat[1][0]=temp;

	//3rd row is right shifted twice.
	temp=mat[2][3];
	mat[2][3]=mat[2][2];
	mat[2][2]=mat[2][1];
	mat[2][1]=mat[2][0];
	mat[2][0]=temp;

	temp=mat[2][3];
	mat[2][3]=mat[2][2];
	mat[2][2]=mat[2][1];
	mat[2][1]=mat[2][0];
	mat[2][0]=temp;
	
	//4th row is right shifted thrice.
	temp=mat[3][3];
	mat[3][3]=mat[3][2];
	mat[3][2]=mat[3][1];
	mat[3][1]=mat[3][0];
	mat[3][0]=temp;

	temp=mat[3][3];
	mat[3][3]=mat[3][2];
	mat[3][2]=mat[3][1];
	mat[3][1]=mat[3][0];
	mat[3][0]=temp;
	
	temp=mat[3][3];
	mat[3][3]=mat[3][2];
	mat[3][2]=mat[3][1];
	mat[3][1]=mat[3][0];
	mat[3][0]=temp;		
}

/*This function peform mix column operation of AES.
**Each column of matrix mat is multiplied with a fixed matrix.
**In GF, addition operation is equivalent to bitwise XOR operation.
**Here, multiplying any hexadecimal character with 0x02 is replaced by logical left shift once and conditional bitwise XOR operation.
**Here, multiplying any hexadecimal character with 0x03 is replaced by multiplication with 0x02 and bitwise XOR operation with hexadecimal **character. */
void mixCol(uch mat[4][4])
{
	uch a[4];						//It will contain column of matrix mat.
	uch b[4];						//It will contain elemnts of a[] multiplied by 0x02.
	int i,j;
	uch h;				//It will contain 0xFF if elements of b[] correspond to polynomial of degree 8, otherwise 0x00.
		
	for(j=0;j<4;j++)
	{
		for (i=0;i<4;i++) 
		{
        		a[i] = mat[i][j];
        		h = (uch)((signed char)mat[i][j] >> 7);		//Arithmetic right shift is performed 7 times. 
        		b[i] = mat[i][j] << 1; 				//Logical left shift is performed once.
        		b[i] = b[i] ^ (0x1B & h);			//bitwise XOR operation and logical AND operation is performed.
        	}

    		mat[0][j] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
    		mat[1][j] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; 
    		mat[2][j] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; 
		mat[3][j] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; 
	}
}

/*void mixCol(uch mat[4][4])
{
	uch a[4];						//It will contain column of matrix mat.
	int i,j;
		
	for(j=0;j<4;j++)
	{
		for (i=0;i<4;i++) 
		{
        		a[i] = mat[i][j];
        		
        	}

    		mat[0][j] = (mul2[a[0]]) ^ (mul3[a[1]]) ^ (a[2]) ^ (a[3]);
    		mat[1][j] = (a[0]) ^ (mul2[a[1]]) ^ (mul3[a[2]]) ^ (a[3]); 
    		mat[2][j] = (a[0]) ^ (a[1]) ^ (mul2[a[2]]) ^ (mul3[a[3]]); 
		mat[3][j] = (mul3[a[0]]) ^ (a[1]) ^ (a[2]) ^ (mul2[a[3]]);
	}
}*/

//This function uses some predifned lookup table in Inverse Mix Column operation while decrypting.
void invMixCol(uch mat[4][4])
{
	uch a[4];						//It will contain column of matrix mat.
	int i,j;
		
	for(j=0;j<4;j++)
	{
		for (i=0;i<4;i++) 
		{
        		a[i] = mat[i][j];
        		
        	}

    		mat[0][j] = (mul14[a[0]]) ^ (mul11[a[1]]) ^ (mul13[a[2]]) ^ (mul9[a[3]]);
    		mat[1][j] = (mul9[a[0]]) ^ (mul14[a[1]]) ^ (mul11[a[2]]) ^ (mul13[a[3]]); 
    		mat[2][j] = (mul13[a[0]]) ^ (mul9[a[1]]) ^ (mul14[a[2]]) ^ (mul11[a[3]]); 
		mat[3][j] = (mul11[a[0]]) ^ (mul13[a[1]]) ^ (mul9[a[2]]) ^ (mul14[a[3]]); 
	}
}
