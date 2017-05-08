
//
//
// Windows OS
//
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
CodePage��ָ��ִ��ת�����ַ����������������Ϊϵͳ�Ѱ�װ����Ч���κ��ַ�����������ֵ��
CP_ACP��ANSI�ַ�����CP_MACCP��Macintosh����ҳ��CP_OEMCP��OEM����ҳ��
CP_SYMBOL�������ַ�����42����CP_THREAD_ACP����ǰ�߳�ANSI����ҳ��
CP_UTF7��ʹ��UTF-7ת����CP_UTF8��ʹ��UTF-8ת����
dwFlags��һ��δ�������ָ���Ƿ�δת����Ԥ������ַ����������ʽ���ڣ����Ƿ�ʹ������������������ַ����Լ���δ�����Ч�ַ���
lpMultiByteStr��ָ�򽫱�ת���ַ������ַ���
cchMultiByte��ָ���ɲ���lpMultiByteStrָ����ַ������ֽڵĸ��������lpMultiByteStrָ�����ַ����Կ��ַ���ֹ����������Ϊ-1������ַ��������Կ��ַ���ֹ������Ϊ-1����ʧ�ܣ����ܳɹ������˲�������Ϊ0������ʧ�ܡ�
lpWideCharStr��ָ����ձ�ת���ַ����Ļ�������
cchWideChar��ָ���ɲ���lpWideCharStrָ��Ļ��������ֽ���������ֵΪ�㣬�������ػ�����������Ŀ��ַ���������������£�lpWideCharStr�еĻ��������� ʹ�á�


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


// UTF-8תGBK
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