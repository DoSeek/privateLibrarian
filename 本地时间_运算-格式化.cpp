
//
// 本地时间格式化
//

/// 方案1: strftime函数
/*
函数原型：我们可以使用strftime()函数将时间格式化为我们想要的格式。它的原型如下：
		size_t strftime( char *str, size_t maxsize, const char *fmt, struct tm *time );

功能：函数按照参数fmt所设定格式将time类型的参数格式化为日期时间信息，然后存储在字符串str中(至多maxsize 个字符)。用于设定时间不同类型的代码为：
参数说明：
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
		%a 星期几的简写										Thu
		%A 星期几的全称										Thursday
		%b 月份的简写											Aug
		%B 月份的全称											August
		%c 标准的日期的时间串									Thu Aug 23 14:55:02 2001	
		%C 年份的后两位数字									20	
		%d 十进制表示的每月的第几天							23			
		%D 月/天/年											08/23/01
		%e 在两字符域中，十进制表示的每月的第几天				23						
		%F 年-月-日											2001-08-23
		%g 年份的后两位数字，使用基于周的年						01				
		%G 年份，使用基于周的年								2001		
		%h 简写的月份名										Aug
		%H 24小时制的小时										14
		%I 12小时制的小时										02
		%j 十进制表示的每年的第几天							235			
		%m 十进制表示的月份									08	
		%M 十时制表示的分钟数									55	
		%n 新行符										    换行
		%p 本地的AM或PM的等价显示								PM		
		%r 12小时的时间										02:55:02 pm
		%R 显示小时和分钟：hh:mm								14:55		
		%S 十进制的秒数										02
		%t 水平制表符											插入一个制表符
		%T 显示时分秒：hh:mm:ss								14:55:02		
		%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）	4									
		%U 第年的第几周，把星期日做为第一天（值从0到53）			33							
		%V 每年的第几周，使用基于周的年							34			
		%w 十进制表示的星期几（值从0到6，星期天为0）				4						
		%W 每年的第几周，把星期一做为第一天（值从0到53）			34							
		%x 标准的日期串										08/23/01
		%X 标准的时间串										14:55:02
		%y 不带世纪的十进制年份（值从0到99）					01					
		%Y 带世纪部分的十制年份								2001		
		%z，													+100
		%Z 时区名称，如果不能得到时区名称则返回空字符。			CDT							
		%% 百分号											%
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