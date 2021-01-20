#include <iostream>
#include <iomanip>
#include <BookManage.h>
void SelectAllBook(MYSQL* MidCarrier);
int SelectOneBook(MYSQL* MidCarrier);
void OutPutBookInfor(book* ResultBook);
void InsertBook(MYSQL* MidCarrier);
date CharChangeDate(char* Origin);
int ReaderOpera(MYSQL* MidCarrier);
int SelectReaderInfor(MYSQL* MidCarrier);
int ReaderToLog(MYSQL* MidCarrier);
void InsertReaderInfor(MYSQL* MidCarrier);
void SelTheReaBook(MYSQL* MidCarrier, char* NumberReader);
void SelectRecord(MYSQL* MidCarrier);
int BrrowBook(MYSQL* MidCarrier);
void ReturnBook(MYSQL* MidCarrier);
void ReNewDate(MYSQL* MidCarrier);
void DeleteReaderInfor(MYSQL* MidCarrier);
int AdminiOpera(MYSQL*MidCarrier);
void RankListReader(MYSQL* MidCarrier);
void RankListBook(MYSQL* MidCarrier);
void BookHadBrrow(MYSQL* MidCarrier);
int SelectAdminInfor(MYSQL* MidCarrier);
int main()
{
    //mysql_init() 获得或初始化一个MYSQL结构。
    //查看是否实例是否创建成功
    if(mysql_init(&mysql) == NULL)
    {
        //mysql_error() 返回最近被调用的MySQL函数的出错消息。
        cout << mysql_error(&mysql);
        return -1;
    }

    //mysql_real_connect("连接的载体地址","服务器名","账户名","数据库名","端口名","字符格式，通常为NULL",0) 连接一个MySQL服务器。
    if(!(MidCarrier = mysql_real_connect(&mysql, "localhost","root","123456","bookmanage",0,NULL,0)))
    {
        //连接数据库，并检查连接是否成功，该数据库是否存在
        cout << mysql_error(&mysql) << endl;
        return -1;
    }
    //初始化
    //设置m_m_hMysql的编码格式
    mysql_query(MidCarrier, "set names 'utf8'");
    SelectResult = NULL;
    m_fd = NULL;
    //Memset() 用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为‘ ’或‘/0’
    memset(SQLSentence, 0, 1024);
    menu();
    cout << "---------------------------------------------------------------------欢迎使用图书管理系统V1.0----------------------------------------------------------------------" << endl;
    int KindPeopleOpera;
    int OpenStatu=0;
    for (int i = 0;; i++)
    {
        cout << "您的身份为（1、读者 2、管理员）：";
        cin >> KindPeopleOpera;
        switch (KindPeopleOpera)
        {
        case 1:
            //用户操作
            ReaderOpera(MidCarrier); break;
        case 2:
            //管理员操作
            AdminiOpera(MidCarrier); break;
        default:
            cout << "---------------选择错误,请重新选择！----------------" << endl;
        }
        cout << "是否退出系统（1、退出 0、继续操作）：";
        cin >> OpenStatu;
        if (OpenStatu)break;
    }
}

void SelectAllBook(MYSQL* MidCarrier)
{
    book* ResultBook = new book;
    int KindToFind;
    //种类选择检错
    for (int i = 1;; i++)
    {
        cout << "你需要查看哪种图书的清单（1、图书 2、报刊 3、期刊）：";
        cin >> KindToFind;
        if (KindToFind < 1 || KindToFind>3)
        {
            cout << "---------没有你想要查询类型的图书哦，请重新选择！-----" << endl;
            continue;
        }
        break;
    }
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()功能是将数据格式化输出到字符串,数字为缓冲区长度
    //将查询语句写入SQLSentence中,类似赋值
    switch (KindToFind)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical"); break;
    default:
        cout << "---------没有你想要查询类型的图书哦，请重新选择！-----" << endl;
    }
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() 检索一个完整的结果集合给客户。 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count返回查询结果行数
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------表格为空！-------------------" << endl;
        }
    }
    cout << TabLightTop << endl;
    OutBookTabName();
    //mysql_fetch_row() 从结果集合中取得下一行
    //atoi(const char* str) 把参数 str 所指向的字符串转换为一个整数
    //  mysql_fetch_row() 偏移量从 0 开始,继续执行该语句自动获取下一行
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultBook->BookNumber, 30, OneRowResult[0]);
            sprintf_s(ResultBook->BookName, 30, OneRowResult[1]);
            sprintf_s(ResultBook->Author, 30, OneRowResult[2]);
            sprintf_s(ResultBook->Publish, 30, OneRowResult[3]);
            ResultBook->PublishDate = CharChangeDate(OneRowResult[4]);
            ResultBook->Price = atoi(OneRowResult[5]);
            sprintf_s(ResultBook->BookKind, 10, OneRowResult[6]);
            ResultBook->WholeBook = atoi(OneRowResult[7]);
            ResultBook->NumNowBook = atoi(OneRowResult[8]);
            ResultBook->NumBorrow = atoi(OneRowResult[9]);
        }
        cout << TabLightMid << endl;
        OutPutBookInfor(ResultBook);
    }
    cout << TabLightButton << endl;

    delete ResultBook;
    mysql_free_result(SelectResult);
}

int SelectOneBook(MYSQL* MidCarrier)
{
    int NumberBook;
    book* ResultOneBook = new book;
    char* BookNameFin = new char[20];
    int KindToFind;
    //种类选择检错
    for (int i = 1;; i++)
    {
        cout << "请输入您要查询图书的种类（1、图书 2、报刊 3、期刊）：";
        cin >> KindToFind;
        if (KindToFind < 1 || KindToFind>3)
        {
            cout << "---------没有你想要查询类型的图书哦，请重新选择！-----" << endl;
            continue;
        }
        break;
    }
    cout << "请输入你要查询的图书的书名：";
    cin >> BookNameFin;
    //查询book
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()功能是将数据格式化输出到字符串,数字为缓冲区长度
    //将查询语句写入SQLSentence中,类似赋值
    switch (KindToFind)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical"); break;
    default:
        cout << "------你的图书种类选择还是有点错误，请按要求来！------" << endl;
    }
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() 检索一个完整的结果集合给客户。 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count返回查询结果行数
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------表格为空！-------------------" << endl;
        }
    }
    //mysql_fetch_row() 从结果集合中取得下一行
    //atoi(const char* str) 把参数 str 所指向的字符串转换为一个整数
    //  mysql_fetch_row() 偏移量从 0 开始,继续执行该语句自动获取下一行
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultOneBook->BookNumber, 30, OneRowResult[0]);
            sprintf_s(ResultOneBook->BookName, 30, OneRowResult[1]);
            sprintf_s(ResultOneBook->Author, 30, OneRowResult[2]);
            sprintf_s(ResultOneBook->Publish, 30, OneRowResult[3]);
            ResultOneBook->PublishDate = CharChangeDate(OneRowResult[4]);
            ResultOneBook->Price = atoi(OneRowResult[5]);
            sprintf_s(ResultOneBook->BookKind, 10, OneRowResult[6]);
            ResultOneBook->WholeBook = atoi(OneRowResult[7]);
            ResultOneBook->NumNowBook = atoi(OneRowResult[8]);
            ResultOneBook->NumBorrow = atoi(OneRowResult[9]);
        }
        if (strcmp(ResultOneBook->BookName, BookNameFin) == 0)
        {
            cout << TabLightTop << endl;
            OutBookTabName();
            cout << TabLightMid << endl;
            OutPutBookInfor(ResultOneBook);
            cout << TabLightButton << endl;
            NumberBook = ResultOneBook->NumNowBook;
            break;
        }
    }
    delete[] BookNameFin;
    delete ResultOneBook;
    mysql_free_result(SelectResult);
    return NumberBook;
}

//SQL插入语句：INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
void InsertBook(MYSQL* MidCarrier)
{
    int KindToInsert;
    //种类选择检错
    for (int i = 1;; i++)
    {
        cout << "您想要添加哪种图书（1、图书 2、报刊 3、期刊）：";
        cin >> KindToInsert;
        if (KindToInsert < 1 || KindToInsert>3)
        {
            cout << "没有你想要查询类型的图书哦！请重新选择。" << endl;
            continue;
        }
        break;
    }
   
    book BookToInsert;
    //用于存放时间
    char* InputDateChar = new char[11];
    cout << "开始输入新添图书信息" << endl;
    cout << "编号：";
    cin >> BookToInsert.BookNumber;
    cout << "书名：";
    cin >> BookToInsert.BookName;
    cout << "作者：";
    cin >> BookToInsert.Author;
    cout << "出版社：";
    cin >> BookToInsert.Publish;
    cout << "出版日期（格式：XXXX-XX-XX）：";
    cin >> InputDateChar;
    cout << "价格：";
    cin >> BookToInsert.Price;
    cout << "类别：";
    cin >> BookToInsert.BookKind;
    cout << "总入库量：";
    cin >> BookToInsert.WholeBook;
    //由于刚添加，所以默认余量等于全部，借出量等于0
    BookToInsert.NumNowBook = BookToInsert.WholeBook;
    BookToInsert.NumBorrow = 0;
    switch (KindToInsert)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "insert into book (编号, 书名, 作者,出版社,出版日期,金额,类别,总入库量,当前库存量,已借出本数) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook, BookToInsert.NumBorrow); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "insert into journal (编号, 书名, 作者,出版社,出版日期,金额,类别,总入库量,当前库存量,已借出本数) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook,BookToInsert.NumBorrow); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "insert into periodical (编号, 书名, 作者,出版社,出版日期,金额,类别,总入库量,当前库存量,已借出本数) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook, BookToInsert.NumBorrow); break;
    default:
        cout << "你的图书种类选择还是有点错误，请按要求来！" << endl;
    }
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "添加成功！" << endl;
    }
    delete[] InputDateChar;
}

//SQL修改表语句：UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
//鉴于三张图书表的特性，对表的更新，此处只涉及更改现有存量和借出量
//借书操作函数先用户登录，再查询，有存量，Insert记录，无存量，查询最近的还书时间
int ReaderOpera(MYSQL* MidCarrier)
{
    int IfKeepGoing = 0;
    int OperaKind;
    int LogOut = 0;
    //先进行读者注册或者登录
    IfKeepGoing=ReaderToLog(MidCarrier);
    if (IfKeepGoing)
    {
        return 1;
    }
    for (int LogSta = 0;; LogSta++)
    {
        cout << "---------------请选择你要进行的操作---------------" << endl;
        cout << "（1、借书 2、还书 3、续借）： ";
        cin >> OperaKind;
        switch (OperaKind)
        {
        case 1:
            BrrowBook(MidCarrier); break;
        case 2:
            ReturnBook(MidCarrier); break;
        case 3:
            ReNewDate(MidCarrier); break;
        default:
            cout << "暂不支持您要进行的操作！" << endl;
        }
        cout << "是否退出登录(1、退出 0、继续操作)：";
        cin >> LogOut;
        if (LogOut) break;
    }
    return 0;
}

//用户登录或注册
int ReaderToLog(MYSQL* MidCarrier)
{
    int IfLogSucess = 0;
    cout << "---------------欢迎来到读者界面，请选择-------------" << endl;
    int KindToLog;
    cout << "请选择（1、登录 2、注册 3、注销账户）：  ";
    cin >> KindToLog;
    //用户证件号
    switch (KindToLog)
    {
    case 1:
        IfLogSucess=SelectReaderInfor(MidCarrier);
        if (IfLogSucess)
        {
            //如果登录失败返回1
            return 1;
        }
        break;
    case 2:
        InsertReaderInfor(MidCarrier);
        return 1;
        break;
    case 3:
        DeleteReaderInfor(MidCarrier); 
        return 1;
        break;
    default:
        cout << "你的登录有点错误，请按要求来！" << endl;
        //如果输入错误值也登录结束操作
        return 1;
    }
    return 0;
}

//用于用户登录
int SelectReaderInfor(MYSQL* MidCarrier)
{
    //状态变量，用于最终查询状态的判定
    int LastJudge = 0;
    char* NumberReader = new char[20];
    char* PassWord1 = new char[20];
    //暂时分配10个类型地址空间
    readerinfor* ResultOneReader = new readerinfor[10];
    cout << "-------------开始登录，请按提示输入信息-------------" << endl;
    cout << "用户名（借书证号）：";
    cin >> NumberReader;
    cout << "              密码：";
    cin >> PassWord1;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from readerinformation");
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    { 
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "---------------表格为空，请选择注册！---------------" << endl;
        }
    }
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultOneReader->CardNumber, 30, OneRowResult[0]);
            sprintf_s(ResultOneReader->ReaderName , 30, OneRowResult[1]);
            sprintf_s(ResultOneReader->ReaderGrade, 30, OneRowResult[2]);
            sprintf_s(ResultOneReader->SchoolNumber, 30, OneRowResult[3]);
            ResultOneReader->ReaderLevel = atoi(OneRowResult[4]);
            sprintf_s(ResultOneReader->PassWord, 30, OneRowResult[5]);
        }
        if ((strcmp(ResultOneReader->CardNumber, NumberReader) || strcmp(ResultOneReader->PassWord, PassWord1))==0)
        {
            LastJudge = 1;
            break;
        }
        else
        {
            continue;
        }
    }
    if (LastJudge)
    {
        cout << "---------------------登录成功---------------------" << endl;
        //提示还没有还的书
        SelTheReaBook(MidCarrier, NumberReader);
        cout << "-------请仔细查看借阅信息，切勿逾期还书哦！-------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneReader;
        return 0;
    }
    else
    {
        cout << "-------------登录失败，密码错误或用户不存在---------- " << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneReader;
        return 1;
    }
}

//用于读者注册（读者信息的录入）
void InsertReaderInfor(MYSQL* MidCarrier)
{
    readerinfor* NewReader = new readerinfor;
    char *PassWordSec = new char[20];
    cout << "-------------------开始注册----------------------" << endl;
    cout << setfill(' ') << setw(10) << "借书证号：";
    cin >> NewReader ->CardNumber;
    cout << setfill(' ') << setw(10) << "姓名：";
    cin >> NewReader->ReaderName;
    cout << setfill(' ') << setw(10) << "班级：";
    cin >> NewReader->ReaderGrade;
    cout << setfill(' ') << setw(10) << "学号：";
    cin >> NewReader->SchoolNumber;
    cout << setfill(' ') << setw(20) << "你的身份（0、学生 1、老师）：";
    cin >> NewReader->ReaderLevel;
    for (int i=1;;i++)
    {
        cout << setfill(' ') << setw(10) << "密码：";
        cin >> NewReader->PassWord;
        cout << setfill(' ') << setw(10) << "再次输入密码：";
        cin >> PassWordSec;
        if (strcmp(NewReader->PassWord, PassWordSec) == 0)
        {
            break;
        }
        else
        {
            cout << "-------------两次输入不一致,请重新输出！-----------" << endl;
        }
    }
    int NumberBrrow = 0;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "insert into readerinformation (借书证号, 姓名, 班级,学号,读者等级,密码,借阅本数) values ('%s', '%s', '%s', '%s', %d, '%s',%d)", NewReader->CardNumber, NewReader->ReaderName, NewReader->ReaderGrade, NewReader->SchoolNumber, NewReader->ReaderLevel, NewReader->PassWord, NumberBrrow);
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "---------------注册成功！请前往登录！--------------" << endl;
    }
    delete  NewReader;
    delete[] PassWordSec;
}

//用于读者注销账户，DELETE FROM 表名称 WHERE 列名称 = 值
void DeleteReaderInfor(MYSQL* MidCarrier)
{
    int IfKeepDelete;
    readerinfor* CancelReader = new readerinfor;
    cout << "-------------------开始注销----------------------" << endl;
    cout << setfill(' ') << setw(10) << "借书证号：";
    cin >> CancelReader->CardNumber;
    cout << setfill(' ') << setw(10) << "密码：";
    cin >> CancelReader->PassWord;
    int NumberBrrow = 0;
    cout << "确定要注销账户吗？（1、确定 0、放弃）：";
    cin >> IfKeepDelete;
    if (IfKeepDelete)
    {
        //+		SQLSentence	0x00007ff7ac6b67f0 "select * from brrowrecord where 借书证号='2020121207' AND 还书日期=NULL"	char[1024]

        mysql_query(MidCarrier, "SET NAMES 'GBK'");
        sprintf_s(SQLSentence, 1024, "select * from brrowrecord where 借书证号='%s' AND 还书日期 is NULL", CancelReader->CardNumber);
        if (mysql_query(MidCarrier, SQLSentence) == 0)
        {
            SelectResult = mysql_store_result(MidCarrier);
            if (SelectResult->row_count >= 1)
            {
                cout << "您当前有" << SelectResult->row_count << "本书未还，请还书后再注销账户！" << endl;
            }
            else
            {
                cout << "您当前无未归还书，可以注销账户！" << endl;
                sprintf_s(SQLSentence, 1024, "delete from readerinformation where 借书证号='%s' AND 密码='%s' ", CancelReader->CardNumber, CancelReader->PassWord);
                if (mysql_query(MidCarrier, SQLSentence) == 0)
                {
                    cout << "-----------------账户注销成功！------------------" << endl;
                }
                else
                {
                    cout << "----------注销失败，请检查借书证号和密码！---------" << endl;
                }
            }
        }
    }
    delete  CancelReader;
}

//查询该用户需要还的书，,用户登录后自动执行，列出所有该登录用户借阅的所有的书，用于提醒用户还书
void SelTheReaBook(MYSQL* MidCarrier, char* NumberReader)
{
    brrowrecord* ResultSelectRecord = new brrowrecord[30];  
    cout << "------------------当前您借阅的书------------------" << endl;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()功能是将数据格式化输出到字符串,数字为缓冲区长度
    //将查询语句写入SQLSentence中,类似赋值
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord where 还书日期 is NULL");
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() 检索一个完整的结果集合给客户。 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count返回查询结果行数
        if (SelectResult->row_count <= 0)
        {
            cout << "借阅记录表为空！" << endl;
        }
    }
    //mysql_fetch_row() 从结果集合中取得下一行
    //atoi(const char* str) 把参数 str 所指向的字符串转换为一个整数
    //  mysql_fetch_row() 偏移量从 0 开始,继续执行该语句自动获取下一行
    cout << TabReaRecLightTop << endl;
    OutReaRecordTabName();
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultSelectRecord->BookName, 30, OneRowResult[1]);
            sprintf_s(ResultSelectRecord->CardNumber, 30, OneRowResult[3]);
            ResultSelectRecord->BorrowDate = CharChangeDate(OneRowResult[4]);
            if (OneRowResult[5]==NULL )
            {
                ResultSelectRecord->ReturnDate.year  = 0000;
                ResultSelectRecord->ReturnDate.month = 00;
                ResultSelectRecord->ReturnDate.day = 00;
            }
            else
            {
                ResultSelectRecord->ReturnDate = CharChangeDate(OneRowResult[5]);
            }
            //暂时先不访问罚款
        }
        if (strcmp(ResultSelectRecord->CardNumber, NumberReader)==0)
        {
            cout << TabReaRecLightMid << endl;
            cout << "│ "  << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " │ " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-"<<setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " │ " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " │" << endl;
        }
    }
    cout << TabReaRecLightButton << endl;
    delete[] ResultSelectRecord;
    mysql_free_result(SelectResult);
}

//插入借阅记录，借书，添加借阅记录，更新书表，更新用户借阅本数
int BrrowBook(MYSQL* MidCarrier)
{
    int IfToBrrow;
    int NumTheReaBrr;
    int KindOpera;
    int NumberNow=0;
    int NumberBrrow=0;
    int ReaderBrrowNow = 1;
    cout << "是否要查看全部图书目录（1、查看 0、直接搜索指定图书）：";
    cin >> KindOpera;
    if (KindOpera)
    {
        SelectAllBook(MidCarrier);
    }
    //先进行图书的查询
    cout << "-------------------待借图书查询------------------" << endl;
    if (SelectOneBook(MidCarrier) <= 0)
    {
        cout << "---------你查询的图书已全部借出,暂无余量!----------" << endl;
    }
    else
    {
        cout << "-----------你查询图书还有余量,可以借阅!-----------" << endl;
        cout << "是否进行借阅（1、借阅 0、不借阅）：";
        cin >> IfToBrrow;
        if (IfToBrrow)
        {
            //便于借阅成功后图书信息表的修改操作
            int KindBook;
            int ReaderLevel;
            brrowrecord *RecordToInsert=new brrowrecord;
            //用于存放时间
            char* InputBrrowDate = new char[11];
            cout << "---------------开始输入图书借阅信息---------------" << endl;
            cout << "请输入您要借阅图书的种类（1、图书 2、报刊 3、期刊）：";
            cin >> KindBook;
            cout << "图书编号：";
            cin >> RecordToInsert->BookNumber;
            cout << "书名：";
            cin >> RecordToInsert->BookName;
            cout << "金额：";
            cin >> RecordToInsert->Price;
            cout << "借书证号：";
            cin >> RecordToInsert->CardNumber;
            cout << "借书日期（格式：XXXX-XX-XX）：";
            cin >> InputBrrowDate;
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            sprintf_s(SQLSentence, 1024, "select * from brrowrecord where 借书证号='%s' AND 还书日期 is NULL", RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count >= 1)
                {
                    cout << "--------------您当前正在借阅" << SelectResult->row_count << "本图书！-------------" << endl;
                }
                else
                {
                    cout << "------------您当前没有正在借阅的图书！------------" << endl;
                }
            }
            NumTheReaBrr = SelectResult->row_count;

            sprintf_s(SQLSentence, 1024, "select 读者等级 from readerinformation where 借书证号='%s' ", RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            { 
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count <= 0)
                {
                    cout << "--------------------无该用户！-------------------" << endl;
                }
                else
                {
                    OneRowResult = mysql_fetch_row(SelectResult);
                    ReaderLevel = atoi(OneRowResult[0]);
                    if (ReaderLevel)
                    {
                        cout << "----------------您的为身份为老师！----------------"<< endl;
                        if (NumTheReaBrr >= 5)
                        {
                            cout << "您的借阅本数已达到上限（5本）,请归还图书后在借阅！" << endl;
                            return 1;
                        }
                        else
                        {
                            cout << "您的借阅本数未达到上限（5本），可以进行借阅！" << endl;
                        }
                    }
                    else
                    {
                        cout << "----------------您的为身份为学生！----------------" << endl;
                        if (NumTheReaBrr >= 3)
                        {
                            cout << "您的借阅本数已达到上限（3本）,请归还图书后再借阅！" << endl;
                            return 1;
                        }
                        else
                        {
                            cout << "您的借阅本数未达到上限（3本），可以进行借阅！" << endl;
                        }
                    }
                }
            }
            Date DateBrrow = CharChangeDate(InputBrrowDate);
            //自动计算还书时间为一个月（30天）后
            Date ReturnDate=DateBrrow;
            if (DateBrrow.month == 12)
            {
                ReturnDate.month = 1;
                ReturnDate.year++;
            }
            else
            {
                ReturnDate.month++;
            }
            //由于刚借阅，所以罚款初始值为0
            RecordToInsert->Fine = 0;
            sprintf_s(SQLSentence, 1024, "insert into brrowrecord (编号, 书名, 金额,借书证号,借书日期,到期日期,罚款金额) values ('%s', '%s', %d, '%s', '%s', '%d-%d-%d', %d)", RecordToInsert->BookNumber, RecordToInsert->BookName, RecordToInsert->Price, RecordToInsert->CardNumber, InputBrrowDate, ReturnDate.year, ReturnDate.month, ReturnDate.day,RecordToInsert->Fine);
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "借阅成功，读书愉快，请在30天内还书哦，否则将会罚款！" << endl;
            }
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            switch (KindBook)
            {
            case 1:
                sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from book where 书名='%s'", RecordToInsert->BookName); break;
            case 2:
                sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from journal where 书名='%s'", RecordToInsert->BookName); break;
            case 3:
                sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from periodical where 书名='%s'", RecordToInsert->BookName); break;
            default:
                cout << "你选择的图书种类有点错误，请按要求来！" << endl;
            }
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count <= 0)
                {
                    cout << "--------------------表格为空！-------------------" << endl;
                }
            }
            for (int i = 0; i < SelectResult->row_count; i++)
            {
                if (OneRowResult = mysql_fetch_row(SelectResult))
                {
                    NumberNow = atoi(OneRowResult[0]);
                    NumberBrrow = atoi(OneRowResult[1]);
                }
            }
            NumberNow = NumberNow - 1;
            NumberBrrow = NumberBrrow + 1;
            switch (KindBook)
            {
            case 1:
                sprintf_s(SQLSentence, 1024, "update book set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow,RecordToInsert->BookName); break;
            case 2:
                sprintf_s(SQLSentence, 1024, "update journal set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow, RecordToInsert->BookName); break;
            case 3:
                sprintf_s(SQLSentence, 1024, "update periodical set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow, RecordToInsert->BookName); break;
            default:
                cout << "你选择的图书种类有点错误，请按要求来！" << endl;
            }
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "------------------图书表更新成功-----------------" << endl;
            }

            sprintf_s(SQLSentence, 1024, "select * from readerinformation where 借书证号='%s'", RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                SelectResult = mysql_store_result(MidCarrier);
            }
            for (int i = 0; i < SelectResult->row_count; i++)
            {
                if (OneRowResult = mysql_fetch_row(SelectResult))
                {
                    ReaderBrrowNow = atoi(OneRowResult[6]);
                }
            }
            ReaderBrrowNow= ReaderBrrowNow+1;
            sprintf_s(SQLSentence, 1024, "update readerinformation set 借阅本数=%d where 借书证号='%s'", ReaderBrrowNow, RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "---------------读者借阅本数更新成功--------------" << endl;
            }
            delete RecordToInsert;
            delete[] InputBrrowDate;
            mysql_free_result(SelectResult);
        }
        else
        {
            cout << "-----------------返回上一级操作!-----------------" << endl;
        }
    }
    return 1;
}

//还书，更新还书时间、更新书表
void ReturnBook(MYSQL* MidCarrier)
{
    cout << "---------------------还书操作---------------------" << endl;
    int FineEachDay = 1;
    int BookKindReturn;
    int NumberNow = 0;
    int NumberBrrow = 0;
    brrowrecord* PapreToReturn = new brrowrecord;
    char* DateToReturn = new char[11];
    char* BrrowDate = new char[11];
    cout << "请选择待还书的种类（1、图书 2、报刊 3、期刊 ）：";
    cin >> BookKindReturn;
    cout << "书名：";
    cin >> PapreToReturn->BookName;
    cout << "借书证号：";
    cin >> PapreToReturn->CardNumber;
    cout << "借书时间（格式：XXXX-XX-XX）：";
    cin >> BrrowDate;
    cout << "请输入当前的时间（格式：XXXX-XX-XX）：";
    cin >> DateToReturn; 
    Date DateReturn = CharChangeDate(DateToReturn);
    Date DateBrrow = CharChangeDate(BrrowDate);
    //为了方便计算，规定每个自然月为30天,每年360天
    if ((DateReturn.year * 360 + DateReturn.month * 30 + DateReturn.day - DateBrrow.year * 360 - DateBrrow.month * 30 - DateBrrow.day - 30) > 0)
    {
        PapreToReturn->Fine = DateReturn.year * 360 + DateReturn.month * 30 + DateReturn.day - DateBrrow.year * 360 - DateBrrow.month * 30 - DateBrrow.day - 30;
        cout << "请支付" << PapreToReturn->Fine << "元罚款！" << endl;
    }
    else
    {
        cout << "规定时间内还书，无需支付罚款!" << endl;
        PapreToReturn->Fine = 0;
    }
    PapreToReturn->Fine = PapreToReturn->Fine * FineEachDay;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "update brrowrecord set 罚款金额=%d,还书日期='%s' where 书名='%s' AND 借书证号='%s' AND 借书日期='%s'", PapreToReturn->Fine, DateToReturn, PapreToReturn->BookName, PapreToReturn->CardNumber, BrrowDate);
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "--------------------还书成功！-------------------" << endl;
        switch (BookKindReturn)
        {
        case 1:
            sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from book where 书名='%s'", PapreToReturn->BookName); break;
        case 2:
            sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from journal where 书名='%s'", PapreToReturn->BookName); break;
        case 3:
            sprintf_s(SQLSentence, 1024, "select 当前库存量,已借出本数 from periodical where 书名='%s'", PapreToReturn->BookName); break;
        default:
            cout << "你选择的图书种类有点错误，请按要求来！" << endl;
        }
        if (mysql_query(MidCarrier, SQLSentence) == 0)
        {
            SelectResult = mysql_store_result(MidCarrier);
        }
        for (int i = 0; i < SelectResult->row_count; i++)
        {
            if (OneRowResult = mysql_fetch_row(SelectResult))
            {
                NumberNow = atoi(OneRowResult[0]);
                NumberBrrow = atoi(OneRowResult[1]);
            }
        }
        NumberNow = NumberNow + 1;
        NumberBrrow = NumberBrrow - 1;
        switch (BookKindReturn)
        {
        case 1:
            sprintf_s(SQLSentence, 1024, "update book set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        case 2:
            sprintf_s(SQLSentence, 1024, "update journal set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        case 3:
            sprintf_s(SQLSentence, 1024, "update periodical set 当前库存量=%d,已借出本数=%d where 书名='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        default:
            cout << "你选择的图书种类有点错误，请按要求来！" << endl;
        }
        if (mysql_query(MidCarrier, SQLSentence) == 0)
        {
            cout << "------------------图书表更新成功-----------------" << endl;
        }
    }
    delete PapreToReturn;
    delete[] DateToReturn;
    delete[] BrrowDate;
}

//续期操作，此处默认续期30天，通过将借阅时间和到期时间后移一个月（默认30天）实现
void ReNewDate(MYSQL* MidCarrier)
{
    cout << "---------------------续期操作---------------------" << endl;
    brrowrecord* PapreToRenew = new brrowrecord;
    char* BrrowDate = new char[11];
    cout << "书名：";
    cin >> PapreToRenew->BookName;
    cout << "借书证号：";
    cin >> PapreToRenew->BookNumber;
    cout << "借书时间（格式：XXXX-XX-XX）：";
    cin >> BrrowDate;
    Date DateBrrow = CharChangeDate(BrrowDate);
    if (DateBrrow.month == 12)
    {
        DateBrrow.month = 1;
        DateBrrow.year++;
    }
    else
    {
        DateBrrow.month++;
    }
    Date DateReturn = DateBrrow;
    if (DateReturn.month == 12)
    {
        DateReturn.month = 1;
        DateReturn.year++;
    }
    else
    {
        DateReturn.month++;
    }
    //+		SQLSentence	0x00007ff78b3817f0 "update brrowrecord set 借书日期='2000-10-5',到期日期='2000-11-5' where 书名='《清史》' AND 借书证号='2020120106' AND 借书日期='2000-09-05'"	char[1024]


    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "update brrowrecord set 借书日期='%d-%d-%d',到期日期='%d-%d-%d' where 书名='%s' AND 借书证号='%s' AND 借书日期='%s'", DateBrrow.year, DateBrrow.month, DateBrrow.day, DateReturn.year, DateReturn.month, DateReturn.day, PapreToRenew->BookName, PapreToRenew->BookNumber, BrrowDate);

    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "--------------------续期成功！-------------------" << endl;
    }
    delete PapreToRenew;
    delete[] BrrowDate;
}

int AdminiOpera(MYSQL* MidCarrier)
{
    int IfLogSucess = 0;
    //管理员登录
    IfLogSucess=SelectAdminInfor(MidCarrier);
    if (IfLogSucess)
    {
        return 0;
    }
    int KindToOpera;
    int LogOutSta;
    for (int LogSta = 0;; LogSta++)
    {
        cout << "---------------请选择你要进行的操作---------------" << endl;
        cout << "（1、新书入库 2、查看全部图书状态 3、查看读者阅读排行 4、查看图书借阅排行 5、查看全部借阅记录 6、查看所有未归还图书）： ";
        cin >> KindToOpera;
        switch (KindToOpera)
        {
        case 1:
            InsertBook(MidCarrier); break;
        case 2:
            SelectAllBook(MidCarrier); break;
        case 3:
            RankListReader(MidCarrier); break;
        case 4:
            RankListBook(MidCarrier); break;
        case 5:
            SelectRecord(MidCarrier); break;
        case 6:
            BookHadBrrow(MidCarrier); break;
        default:
            cout << "-------------暂不支持您要进行的操作！-------------" << endl;
        }
        cout << "是否退出登录(1、退出 0、继续操作)：";
        cin >> LogOutSta;
        if (LogOutSta) break;
    }
    return 0;
}

//查询全部的借阅记录,管理员使用
void SelectRecord(MYSQL* MidCarrier)
{
    brrowrecord* ResultSelectRecord = new brrowrecord[10];
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()功能是将数据格式化输出到字符串,数字为缓冲区长度
    //将查询语句写入SQLSentence中,类似赋值
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord ");
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() 检索一个完整的结果集合给客户。 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count返回查询结果行数
        if (SelectResult->row_count <= 0)
        {
            cout << "----------------借阅记录表为空！----------------" << endl;
        }
    }
    //mysql_fetch_row() 从结果集合中取得下一行
    //atoi(const char* str) 把参数 str 所指向的字符串转换为一个整数
    //  mysql_fetch_row() 偏移量从 0 开始,继续执行该语句自动获取下一行
    cout << TabRecordLightTop << endl;
    OutRecordTabName();
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultSelectRecord->BookNumber, 30, OneRowResult[0]);
            sprintf_s(ResultSelectRecord->BookName, 30, OneRowResult[1]);
            ResultSelectRecord->Price = atoi(OneRowResult[2]);
            sprintf_s(ResultSelectRecord->CardNumber, 30, OneRowResult[3]);
            ResultSelectRecord->BorrowDate = CharChangeDate(OneRowResult[4]);
            ResultSelectRecord->ReturnDate = CharChangeDate(OneRowResult[5]);
            //暂时先不访问罚款
        }
        cout << TabRecordLightMid << endl;
        cout << "│ " << setfill(' ') << setw(20) << ResultSelectRecord->BookNumber << " │ " << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " │ " << setfill(' ') << setw(4) << ResultSelectRecord->Price << " │ " << setfill(' ') << setw(20) << ResultSelectRecord->CardNumber << " │ " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " │ " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " │" << endl;
    }
    cout << TabRecordLightButton << endl;
    delete[] ResultSelectRecord;
    mysql_free_result(SelectResult);
}

//查看读者借阅排行
void RankListReader(MYSQL* MidCarrier)
{
    //语句设置按照借阅本书排序，所以只需顺序输出即可
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from readerinformation order by 借阅本数 desc");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    { 
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "----------------借阅记录表为空！----------------" << endl;
        }
    }
    cout << TabRankListTop << endl;
    cout << "│ " << setfill(' ') << setw(20) << "姓名" << " │ " << setfill(' ') << setw(10) << "借阅本书" << " │" << endl;
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        readerinfor* RankListReader = new readerinfor;
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(RankListReader->ReaderName, 30, OneRowResult[1]);
            RankListReader->NumberBrrow = atoi(OneRowResult[6]);
        }
        cout << TabRankListMid << endl;
        cout << "│ " << setfill(' ') << setw(20) << RankListReader->ReaderName << " │ " << setfill(' ') << setw(10) << RankListReader->NumberBrrow << " │" << endl;
        delete RankListReader;
    }
    cout << TabRankListButton << endl;
    mysql_free_result(SelectResult);
}

//查看被借阅图书排行
void RankListBook(MYSQL* MidCarrier) 
{
    int KindBook;
    cout << "您想要查看那种图书的借阅排行（1、图书 2、报刊 3、期刊）：";
    cin >> KindBook;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()功能是将数据格式化输出到字符串,数字为缓冲区长度
    //将查询语句写入SQLSentence中,类似赋值
    switch (KindBook)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book order by 已借出本数 desc"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal order by 已借出本数 desc"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical order by 已借出本数 desc"); break;
    default:
        cout << "你的图书种类选择还是有点错误，请按要求来！" << endl;
    }
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() 检索一个完整的结果集合给客户。 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count返回查询结果行数
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------表格为空！-------------------" << endl;
        }
    }
    cout << TabRankListTop << endl;
    cout << "│ " << setfill(' ') << setw(20) << "书名" << " │ " << setfill(' ') << setw(10) << "已借出本数" << " │" << endl;
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        book* RankListBook = new book;
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(RankListBook->BookName, 30, OneRowResult[1]);
            RankListBook->NumBorrow = atoi(OneRowResult[9]);
        }
        cout << TabRankListMid << endl;
        cout << "│ "  << setfill(' ') << setw(20) << RankListBook->BookName << " │ " << setfill(' ') << setw(10) << RankListBook->NumBorrow << " │" << endl;
        delete RankListBook;
    }
    cout << TabRankListButton << endl;
    mysql_free_result(SelectResult);

}

//查看已借出未还的图书信息
void BookHadBrrow(MYSQL* MidCarrier)
{
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord ");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "-----------------借阅记录表为空！----------------" << endl;
        }
    }
    cout << TabRecordLightTop << endl;
    OutRecordTabName();
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        brrowrecord* ResultSelectRecord = new brrowrecord;
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultSelectRecord->BookNumber, 30, OneRowResult[0]);
            sprintf_s(ResultSelectRecord->BookName, 30, OneRowResult[1]);
            ResultSelectRecord->Price = atoi(OneRowResult[2]);
            sprintf_s(ResultSelectRecord->CardNumber, 30, OneRowResult[3]);
            ResultSelectRecord->BorrowDate = CharChangeDate(OneRowResult[4]);
            ResultSelectRecord->ReturnDate = CharChangeDate(OneRowResult[5]);
            if (!OneRowResult[7])
            {
                cout << TabRecordLightMid << endl;
                cout << "│ " << setfill(' ') << setw(20) << ResultSelectRecord->BookNumber << " │ " << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " │ " << setfill(' ') << setw(4) << ResultSelectRecord->Price << " │ " << setfill(' ') << setw(20) << ResultSelectRecord->CardNumber << " │ " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " │ " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " │" << endl;
            }
            delete ResultSelectRecord;
        }
    }
    cout << TabRecordLightButton << endl;
    mysql_free_result(SelectResult);
}

//管理员登陆
int SelectAdminInfor(MYSQL* MidCarrier)
{
    //状态变量，用于最终查询状态的判定
    int LastJudge = 0;
    char* NumberReader = new char[20];
    char* PassWord1 = new char[20];
    //暂时分配10个类型地址空间
    admini* ResultOneAdmin = new admini[10];
    cout << "-------------------管理员登录界面------------------" << endl;
    cout << "用户名（证件号）：";
    cin >> NumberReader;
    cout << "            密码：";
    cin >> PassWord1;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from administrator");
    //mysql_query() 执行指定为一个空结尾的字符串的SQL查询。 
    //执行查询语句,并将结果存储到SelectResult中
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------表格为空，请选择注册！--------------" << endl;
        }
    }
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(ResultOneAdmin->CardNumber, 30, OneRowResult[0]);
            sprintf_s(ResultOneAdmin->PassWord, 30, OneRowResult[2]);
        }
        if ((strcmp(ResultOneAdmin->CardNumber, NumberReader) || strcmp(ResultOneAdmin->PassWord, PassWord1)) == 0)
        {
            LastJudge = 1;
            break;
        }
        else
        {
            continue;
        }
    }
    if (LastJudge)
    {
        cout << "--------------------登录成功！-------------------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneAdmin;
        mysql_free_result(SelectResult);
        return 0;
    }
    else
    {
        cout << "-------------登录失败，用户名或密码错误-----------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneAdmin;
        mysql_free_result(SelectResult);
        //如果登录失败直接不能执行其它操作
        return 1;
    }
}

//用于字符串向date类型转换
//ASCII 0=48
date CharChangeDate(char* Origin)
{
    date DateNow;
    DateNow.year = (int(Origin[0])-48)*1000 + (int(Origin[1])-48) * 100 +(int (Origin[2])-48) * 10 + (int(Origin[3])-48);
    DateNow.month = (int(Origin[5])-48) * 10 + int(Origin[6])-48;
    DateNow.day = (int(Origin[8])-48) * 10 + int(Origin[9])-48;
    return DateNow;
}

//输出图书查询结果
void OutPutBookInfor(book* ResultBook)
{
    cout << "│ " << setfill(' ') << setw(20) << ResultBook->BookNumber << " │ " << setfill(' ') << setw(20) << ResultBook->BookName << " │ " << setfill(' ') << setw(20) << ResultBook->Author << " │ " << setfill(' ') << setw(20) << ResultBook->Publish << " │ " << setfill(' ') << setw(4) << ResultBook->PublishDate.year << "-" << setfill('0') << setw(2) << ResultBook->PublishDate.month << "-" << setfill('0') << setw(2) << ResultBook->PublishDate.day << " │ " << setfill(' ') << setw(4) << ResultBook->Price << " │ " << setfill(' ') << setw(4) << ResultBook->BookKind << " │ " << setfill(' ') << setw(8) << ResultBook->WholeBook << " │ " << setfill(' ') << setw(10) << ResultBook->NumNowBook << " │ " << setfill(' ') << setw(10) << ResultBook->NumBorrow << " │" << endl;

}
