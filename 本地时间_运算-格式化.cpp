
//
// ����ʱ���ʽ��
//

/// ����1: strftime����
/*
����ԭ�ͣ����ǿ���ʹ��strftime()������ʱ���ʽ��Ϊ������Ҫ�ĸ�ʽ������ԭ�����£�
		size_t strftime( char *str, size_t maxsize, const char *fmt, struct tm *time );

���ܣ��������ղ���fmt���趨��ʽ��time���͵Ĳ�����ʽ��Ϊ����ʱ����Ϣ��Ȼ��洢���ַ���str��(����maxsize ���ַ�)�������趨ʱ�䲻ͬ���͵Ĵ���Ϊ��
����˵����
timeptr
	Pointer to a tm structure that contains a calendar time broken down into its components (see struct tm).
ptr
	Pointer to the destination array where the resulting C string is copied.
maxsize
	Maximum number of characters to be copied to ptr, including the terminating null-character.
format
	C string containing any combination of regular characters and special format specifiers. 
	These format specifiers are replaced by the function to the corresponding values to represent the time specified in timeptr. 
	They all begin with a percentage (%) sign, and are:

		+-+-----------------------------------------------+-----------------+
		%a ���ڼ��ļ�д										Thu
		%A ���ڼ���ȫ��										Thursday
		%b �·ݵļ�д											Aug
		%B �·ݵ�ȫ��											August
		%c ��׼�����ڵ�ʱ�䴮									Thu Aug 23 14:55:02 2001	
		%C ��ݵĺ���λ����									20	
		%d ʮ���Ʊ�ʾ��ÿ�µĵڼ���							23			
		%D ��/��/��											08/23/01
		%e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���				23						
		%F ��-��-��											2001-08-23
		%g ��ݵĺ���λ���֣�ʹ�û����ܵ���						01				
		%G ��ݣ�ʹ�û����ܵ���								2001		
		%h ��д���·���										Aug
		%H 24Сʱ�Ƶ�Сʱ										14
		%I 12Сʱ�Ƶ�Сʱ										02
		%j ʮ���Ʊ�ʾ��ÿ��ĵڼ���							235			
		%m ʮ���Ʊ�ʾ���·�									08	
		%M ʮʱ�Ʊ�ʾ�ķ�����									55	
		%n ���з�										    ����
		%p ���ص�AM��PM�ĵȼ���ʾ								PM		
		%r 12Сʱ��ʱ��										02:55:02 pm
		%R ��ʾСʱ�ͷ��ӣ�hh:mm								14:55		
		%S ʮ���Ƶ�����										02
		%t ˮƽ�Ʊ��											����һ���Ʊ��
		%T ��ʾʱ���룺hh:mm:ss								14:55:02		
		%u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��	4									
		%U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��			33							
		%V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���							34			
		%w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��				4						
		%W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��			34							
		%x ��׼�����ڴ�										08/23/01
		%X ��׼��ʱ�䴮										14:55:02
		%y �������͵�ʮ������ݣ�ֵ��0��99��					01					
		%Y �����Ͳ��ֵ�ʮ�����								2001		
		%z��													+100
		%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���			CDT							
		%% �ٷֺ�											%
		+-+-----------------------------------------------+-----------------+

		+-+-------------------------------------------------------------------------------+------------------+
	specifier		Replaced by															  Example
		%a	Abbreviated weekday name *														Thu
		%A	Full weekday name * 															Thursday
		%b	Abbreviated month name *														Aug
		%B	Full month name *																August
		%c	Date and time representation *													Thu Aug 23 14:55:02 2001
		%C	Year divided by 100 and truncated to integer (00-99)							20
		%d	Day of the month, zero-padded (01-31)											23
		%D	Short MM/DD/YY date, equivalent to %m/%d/%y										08/23/01
		%e	Day of the month, space-padded ( 1-31)											23
		%F	Short YYYY-MM-DD date, equivalent to %Y-%m-%d									2001-08-23
		%g	Week-based year, last two digits (00-99)										01
		%G	Week-based year																	2001
		%h	Abbreviated month name * (same as %b)											Aug
		%H	Hour in 24h format (00-23)														14
		%I	Hour in 12h format (01-12)														02
		%j	Day of the year (001-366)														235
		%m	Month as a decimal number (01-12)												08
		%M	Minute (00-59)																	55
		%n	New-line character ('\n')														\n
		%p	AM or PM designation															PM
		%r	12-hour clock time *															02:55:02 pm
		%R	24-hour HH:MM time, equivalent to %H:%M											14:55
		%S	Second (00-61)																	02
		%t	Horizontal-tab character ('\t')													\t
		%T	ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S							14:55:02
		%u	ISO 8601 weekday as number with Monday as 1 (1-7)								4
		%U	Week number with the first Sunday as the first day of week one (00-53)			33
		%V	ISO 8601 week number (00-53)													34
		%w	Weekday as a decimal number with Sunday as 0 (0-6)								4
		%W	Week number with the first Monday as the first day of week one (00-53)			34
		%x	Date representation *															08/23/01
		%X	Time representation *															14:55:02
		%y	Year, last two digits (00-99)													01
		%Y	Year																			2001
		%z	ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100)
			If timezone cannot be determined, no characters									+100
		%Z	Timezone name or abbreviation *
			If timezone cannot be determined, no characters									CDT
		%%	A % sign																		%
		+-+-------------------------------------------------------------------------------+------------------+
		* The specifiers marked with an asterisk (*) are locale-dependent.
		Note: Yellow rows indicate specifiers and sub-specifiers introduced by C99. Since C99, two locale-specific modifiers can 
		also be inserted between the percentage sign (%) and the specifier proper to request an alternative format, where applicable:

		Modifier						Meaning									Applies to
			E			Uses the locale's alternative representation		%Ec %EC %Ex %EX %Ey %EY
			O			Uses the locale's alternative numeric symbols		%Od %Oe %OH %OI %Om %OM %OS %Ou %OU %OV %Ow %OW %Oy
*/
#include <ctime>

char tAll[255];
std::memset(tAll, 0, sizeof(tAll));
std::time_t tm = std::time(0);
std::strftime(tAll, sizeof(tAll), "%Y%m%d%H%M%S", std::localtime(&tm));