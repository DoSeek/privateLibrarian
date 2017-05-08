//
// ˵���� �ռ����������ַ�����UTF-8����ת���Ĵ��뷽��
// ���ߣ� D.M.H
// ���ڣ� 2017.05.08
//



//
// Unix/Linux OS
//
//






//
// Windows OS 
// ��Windows OS�¿��ַ����ת�������϶���ʹ�á�MultiByteToWideChar/WideCharToMultiByte���������������Լ�Windows OS�¡�����ҳ�����
//
// ����� #include <windows.h>
//


/* ���ã�http://www.cnblogs.com/sz-leez/p/4085035.html
[1].MultiByteToWideChar
�������ܣ��ú���ӳ��һ���ַ�����һ�����ַ���unicode�����ַ�����
����ԭ�ͣ�
    int MultiByteToWideChar(   
    ����UINT CodePage,   
    ����DWORD dwFlags,   
    ����LPCSTR lpMultiByteStr,   
    ����int cchMultiByte,   
    ����LPWSTR lpWideCharStr,   
    ����int cchWideChar   
    ����);  

����������
CodePage��
	ָ��ִ��ת�����ַ���(����ҳ)�������������Ϊϵͳ�Ѱ�װ����Ч���κ��ַ�����������ֵ��
			CP_ACP��ANSI�ַ�����
			CP_MACCP��Macintosh����ҳ��
			CP_OEMCP��OEM����ҳ��
			CP_SYMBOL�������ַ�����42����
			CP_THREAD_ACP����ǰ�߳�ANSI����ҳ��
			CP_UTF7��ʹ��UTF-7ת����
			CP_UTF8��ʹ��UTF-8ת����

dwFlags��
	һ��δ�������ָ���Ƿ�δת����Ԥ������ַ����������ʽ���ڣ����Ƿ�ʹ������������������ַ����Լ���δ�����Ч�ַ���

lpMultiByteStr��
	ָ�򽫱�ת���ַ������ַ���

cchMultiByte��
	ָ���ɲ���lpMultiByteStrָ����ַ������ֽڵĸ��������lpMultiByteStrָ�����ַ����Կ��ַ���ֹ����������
	Ϊ-1������ַ��������Կ��ַ���ֹ������Ϊ-1����ʧ�ܣ����ܳɹ������˲�������Ϊ0������ʧ�ܡ�

lpWideCharStr��
	ָ����ձ�ת���ַ����Ļ�������

cchWideChar��
	ָ���ɲ���lpWideCharStrָ��Ļ��������ֽ���������ֵΪ�㣬�������ػ�����������Ŀ��ַ���������������£�lpWideCharStr�еĻ��������� ʹ�á�


[2].WideCharToMultiByte
�������ܣ��ú���ӳ��һ��unicode�ַ�����һ�����ֽ��ַ�����
����ԭ��:
    int WideCharToMultiByte(   
    ����UINT CodePage,          //ָ��ִ��ת���Ĵ���ҳ   
    ����DWORD dwFlags,   
    ����LPCWSTR lpWideCharStr, //ָ���Ŀ��ֽ��ַ����Ļ�����   
    ����int cchWideChar,       //ָ���ɲ���lpWideCharStrָ��Ļ��������ַ�����   
    ����LPSTR lpMultiByteStr,  //ָ����ձ�ת���ַ����Ļ�����   
    ����int cchMultiByte,      //ָ���ɲ���lpMultiByteStrָ��Ļ��������ֵ   
    ����LPCSTR lpDefaultChar,   
    ����LPBOOL pfUsedDefaultChar   
    ����);   

*ע��*������������������ָ��lpMultiByteStr��lpWideCharStr���벻һ��������������ʧ��
*/

// GBKתUTF-8
string ConvertGBK2UTF8(string& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	WCHAR *wszUtf8 = new WCHAR[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);

	string strUTF8(szUtf8);
	delete[] szUtf8;
	delete[] wszUtf8;

	return strUTF8;
}


// UTF-8תGBK
string ConvertUTF82GBK(string& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	WCHAR *wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str() , -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(seGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	string szGBK(strUtf8);
	delete[] szGBK;
	delete[] wszGBK;

	return szGBK;
}

// GB2312תUTF-8
string GB23122UTF8(const char* str)
{
	string result;
	WCHAR *strSrc;
	TCHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i + 1];
	int j = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

//UTF-8תGB2312
string UTF82GB2312(const char* str)
{
	string result;
	WCHAR *strSrc;
	TCHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}
