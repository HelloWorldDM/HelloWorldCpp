#include <stdio.h>
#include <winsock.h>
#include <mysql.h>
//locale.h 头文件定义了特定地域的设置，比如日期格式和货币符号。
#include <locale.h>
#include <vector>
#include <windows.h>
using namespace std;

typedef struct Date {
	int year;
	int month;
	int day;
}date;

//book类型
typedef struct books {
	//编号，书名，作者，出版社，出版日期，价格，书种类，总库存量，现有存量，借出量
	char	BookNumber[20];
	char	BookName[20];
	char	Author[20];
	char    Publish[20];
	date    PublishDate;
	int     Price;
	char    BookKind[10];
	int     WholeBook;
	int     NumNowBook;
	int     NumBorrow;
} book;

typedef book journal;     //报刊类
typedef book periodical;  //期刊类

//借书记录
typedef struct BrrowRecords {
	//编号，书名，金额，借书证号，借书日期，归还日期，罚款金额
	char	BookNumber[20];
	char	BookName[20];
	int     Price;
	char    CardNumber[20];
	date    BorrowDate;
	date    ReturnDate;
	int     Fine;
	date    BackDate;  //?????????后加
} brrowrecord;


//读者信息
typedef struct ReaderInformation {
	//借书证号，读者名，班级，学号，读者等级，
	char    CardNumber[20];
	char    ReaderName[20];
	char    ReaderGrade[20];
	char    SchoolNumber[20];
	int     ReaderLevel;
	char    PassWord[20];
	int     NumberBrrow;
} readerinfor;

typedef struct AdminInfor {
	char    CardNumber[20];
	char    Name[10];
	char    PassWord[20];
} admini;

MYSQL        mysql;
MYSQL*       MidCarrier;		           //定义数据库连接的句柄，它被用于几乎所有的 MySQL函数，类似中间载体
MYSQL_RES*   SelectResult;           //查询结果集
MYSQL_FIELD* m_fd;		           // 包含字段信息的结构
MYSQL_ROW	 OneRowResult;		   // 存放一行查询结果的字符串数组
MYSQL_RES*   res = NULL;
char         SQLSentence[1024];	           //存放查询SQL语句字符串

string TabLightTop = "┌──────────────────────┬──────────────────────┬──────────────────────┬──────────────────────┬────────────┬──────┬──────┬──────────┬────────────┬────────────┐";
string TabLightMid = "├──────────────────────┼──────────────────────┼──────────────────────┼──────────────────────┼────────────┼──────┼──────┼──────────┼────────────┼────────────┤";
string TabLightButton = "└──────────────────────┴──────────────────────┴──────────────────────┴──────────────────────┴────────────┴──────┴──────┴──────────┴────────────┴────────────┘";
string TabRecordLightTop =
"┌──────────────────────┬──────────────────────┬──────┬──────────────────────┬────────────┬────────────┐";
string TabRecordLightMid =
"├──────────────────────┼──────────────────────┼──────┼──────────────────────┼────────────┼────────────┤";
string TabRecordLightButton =
"└──────────────────────┴──────────────────────┴──────┴──────────────────────┴────────────┴────────────┘";
string TabReaRecLightTop =
"┌──────────────────────┬────────────┬────────────┐";
string TabReaRecLightMid =
"├──────────────────────┼────────────┼────────────┤";
string TabReaRecLightButton =
"└──────────────────────┴────────────┴────────────┘";
string TabRankListTop=
"┌──────────────────────┬────────────┐";
string TabRankListMid=
"├──────────────────────┼────────────┤";
string TabRankListButton =
"└──────────────────────┴────────────┘";

//输出表的第一列
void OutBookTabName()
{
	cout << "│ " << setfill(' ') << setw(20) << "编号" << " │ " << setfill(' ') << setw(20) << "书名" << " │ " << setfill(' ') << setw(20) << "作者" << " │ " << setfill(' ') << setw(20) << "出版社" << " │ " << setfill(' ') << setw(10) << "出版日期" << " │ " << setfill(' ') << setw(4) << "金额" << " │ " << setfill(' ') << setw(4) << "类别" << " │ " << setfill(' ') << setw(4) << "总入库量" << " │ " << setfill(' ') << setw(4) << "当前库存量" << " │ " << setfill(' ') << setw(4) << "已借出本数" << " │" << endl;
}
void OutRecordTabName()
{
	cout << "│ " << setfill(' ') << setw(20) << "编号" << " │ " << setfill(' ') << setw(20) << "书名" << " │ " << setfill(' ') << setw(4) << "金额" << " │ " << setfill(' ') << setw(20) << "借书证号" << " │ " << setfill(' ') << setw(10) << "借书日期" << " │ " << setfill(' ') << setw(10) << "到期日期" << " │" << endl;
}
void OutReaRecordTabName()
{
	cout << "│ " << setfill(' ') << setw(20) << "书名" << " │ " << setfill(' ') << setw(10) << "借书日期" << " │ " << setfill(' ') << setw(10) << "到期日期" << " │" << endl;
}

void menu() {

	cout << " 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　" << endl;
	cout << "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　" << endl;
	cout << "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　１　　１１１　　　　　　　　　１１　　　　　　１１　　　　　　　　　　　　　　　　　　　　　　　　　　　　　" << endl;
	cout << "　　　１１１１１１１１１１１１１１１１　　　　　　　　　　　１　　　１１１　　　　　　　　１１１１１１１１１１１１１１１１　　　１１１１１１１１１１１１１１１１１　　　" << endl;
	cout << "　　　１１　　１１１　　　　　　　１１　　　　　　１１１１１１１１１１１１１　　　　　　１１１１１　　　１１　１１　　　　　　　　　　１　　　１１　　１　　　１１　　　" << endl;
	cout << "　　　１　　　１１１１１１１１１　　１　　　　　　　　　　　１　　　１１１１１　　　　１１１　　１１１１１　　　１１　　　　　　　　　１　　　１　　　１　　　　１　　　" << endl;
	cout << "　　　１　　１１１　　　　１１１　　１　　　　　　　　　　　１　　　　１１　　　　　　　　　　　１１１１　　　　１１　　　　　　　　　１　　　１　　　１　　　　１　　　" << endl;
	cout << "　　　１１１１１１１１　　１１　　　１　　　　　　　　　　　１　　　　１　　　　　　　　１１１１１１１１１１１１１１１１１　　　　　　１　　　１１１１１１１１１１　　　" << endl;
	cout << "　　　１１１　　　１１１１１　　　　１　　　　　　　　　　　１　　　　１　　　　　　　　１１　　　　　　　　　　　　　１１　　　　　　１　　　１　　　１　　　　１　　　" << endl;
	cout << "　　　１　　　　　１１１１１　　　　１　　　　　　　　　　　１　　　　１　　　　　　　　１　１１１１１１１１１１１１　１１　　　　　　１　　　１　　　１　　　　１　　　" << endl;
	cout << "　　　１　　　１１１１１１１１１　　１　　　　１１１１１１１１１１１１１１１１　　　　　１　１１　　　　　　　　　１　１１　　　１１１１１１１１１　　１　　　１１　　　" << endl;
	cout << "　　　１１１１１１１１　　１１１１１１　　　　　　　　　　　１　　　　　　１１　　　　　　　１　　　　　　　　　　１　　　　　　　　　１　　　１１１１１１１１１１　　　" << endl;
	cout << "　　　１１１　　　１１１１　　　１１１　　　　　　　　　　　１　　　　　　　１　　　　　　　１１１１１１１１１１１１　　　　　　　　　１　　　　　　　１　　　　　　　　" << endl;
	cout << "　　　１　　　　　　　１１１１　　　１　　　　　　　　　　　１　　　　　　　１　　　　　　　１　　　　　　　　　　　　　　　　　　　　１　　　　　　　１　　　　　　　　" << endl;
	cout << "　　　１　　　１１１１　　１１　　　１　　　　　　　　　　　１　　　　　　　１　　　　　　　１１１１１１１１１１１１１　　　　　　　　１　　　１１１１１１１１１１１　　" << endl;
	cout << "　　　１　　　１１１１１１　　　　　１　　　　　　　　　　　１　　　　　　　１　　　　　　　１　　　　　　　　　　１１　　　　　　　　１　１１　　　　１　　　　　　　　" << endl;
	cout << "　　　１　　　　　　１１１１１　　　１　　　　　　　　　　　１　　　　　　１１　　　　　　　１　　　　　　　　　　　１　　　　　　　　１１１１　　　　１　　　　　　　　" << endl;
	cout << "　　　１　　　　　　　　　１１　　１１　　　　　　　　　　　１　　１　　　１１　　　　　　　１　　　　　　　　　　　１　　　　　　１１１１１　　　　　１　　　　　　　　" << endl;
	cout << "　　　１１１１１１１１１１１１１１１１　　　　　　　　　　　１　１１１１１１１　　　　　　　１１　　　　　　　　　１１　　　　　１１１１　１１１１１１１１１１１１１　　" << endl;
	cout << "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　１　　１１１１　　　　　　　　　１１１１１１１１１１１１１　　　　　１　　　　　　　　　　　　　　　　　　　" << endl;
	cout << " 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　" << endl;
}