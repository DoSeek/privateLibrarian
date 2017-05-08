
//
//
// Windows OS
//
//

/* 引用：http://www.cnblogs.com/sz-leez/p/4085035.html
[1].MultiByteToWideChar
函数功能：该函数映射一个字符串到一个宽字符（unicode）的字符串。
函数原型：
    int MultiByteToWideChar(   
    　　UINT CodePage,   
    　　DWORD dwFlags,   
    　　LPCSTR lpMultiByteStr,   
    　　int cchMultiByte,   
    　　LPWSTR lpWideCharStr,   
    　　int cchWideChar   
    　　);  


函数参数：
CodePage：指定执行转换的字符集，这个参数可以为系统已安装或有效的任何字符集所给定的值。
CP_ACP：ANSI字符集；CP_MACCP：Macintosh代码页；CP_OEMCP：OEM代码页；
CP_SYMBOL：符号字符集（42）；CP_THREAD_ACP：当前线程ANSI代码页；
CP_UTF7：使用UTF-7转换；CP_UTF8：使用UTF-8转换。
dwFlags：一组未标记用以指出是否未转换成预作或宽字符（若组合形式存在），是否使用象形文字替代控制字符，以及如何处理无效字符。
lpMultiByteStr：指向将被转换字符串的字符。
cchMultiByte：指定由参数lpMultiByteStr指向的字符串中字节的个数。如果lpMultiByteStr指定的字符串以空字符终止，可以设置为-1（如果字符串不是以空字符中止，设置为-1可能失败，可能成功），此参数设置为0函数将失败。
lpWideCharStr：指向接收被转换字符串的缓冲区。
cchWideChar：指定由参数lpWideCharStr指向的缓冲区的字节数。若此值为零，函数返回缓冲区所必需的宽字符数，在这种情况下，lpWideCharStr中的缓冲区不被 使用。


[2].WideCharToMultiByte
函数功能：该函数映射一个unicode字符串到一个多字节字符串。
函数原型:
    int WideCharToMultiByte(   
    　　UINT CodePage,          //指定执行转换的代码页   
    　　DWORD dwFlags,   
    　　LPCWSTR lpWideCharStr, //指定的宽字节字符串的缓冲区   
    　　int cchWideChar,       //指定由参数lpWideCharStr指向的缓冲区的字符个数   
    　　LPSTR lpMultiByteStr,  //指向接收被转换字符串的缓冲区   
    　　int cchMultiByte,      //指定由参数lpMultiByteStr指向的缓冲区最大值   
    　　LPCSTR lpDefaultChar,   
    　　LPBOOL pfUsedDefaultChar   
    　　);   

*注意*：以上两个函数参数指针lpMultiByteStr和lpWideCharStr必须不一样，否则函数返回失败
*/

// GBK转UTF-8
void ConvertGBKToUtf8(CString& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
	WCHAR *wszUtf8 = new WCHAR[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
}


// UTF-8转GBK
void ConvertUtf8ToGBK(CString& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
	WCHAR *wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(seGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}