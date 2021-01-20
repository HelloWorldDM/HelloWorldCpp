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
    //mysql_init() ��û��ʼ��һ��MYSQL�ṹ��
    //�鿴�Ƿ�ʵ���Ƿ񴴽��ɹ�
    if(mysql_init(&mysql) == NULL)
    {
        //mysql_error() ������������õ�MySQL�����ĳ�����Ϣ��
        cout << mysql_error(&mysql);
        return -1;
    }

    //mysql_real_connect("���ӵ������ַ","��������","�˻���","���ݿ���","�˿���","�ַ���ʽ��ͨ��ΪNULL",0) ����һ��MySQL��������
    if(!(MidCarrier = mysql_real_connect(&mysql, "localhost","root","123456","bookmanage",0,NULL,0)))
    {
        //�������ݿ⣬����������Ƿ�ɹ��������ݿ��Ƿ����
        cout << mysql_error(&mysql) << endl;
        return -1;
    }
    //��ʼ��
    //����m_m_hMysql�ı����ʽ
    mysql_query(MidCarrier, "set names 'utf8'");
    SelectResult = NULL;
    m_fd = NULL;
    //Memset() ������һ���ڴ�ռ�ȫ������Ϊĳ���ַ���һ�����ڶԶ�����ַ������г�ʼ��Ϊ�� ����/0��
    memset(SQLSentence, 0, 1024);
    menu();
    cout << "---------------------------------------------------------------------��ӭʹ��ͼ�����ϵͳV1.0----------------------------------------------------------------------" << endl;
    int KindPeopleOpera;
    int OpenStatu=0;
    for (int i = 0;; i++)
    {
        cout << "�������Ϊ��1������ 2������Ա����";
        cin >> KindPeopleOpera;
        switch (KindPeopleOpera)
        {
        case 1:
            //�û�����
            ReaderOpera(MidCarrier); break;
        case 2:
            //����Ա����
            AdminiOpera(MidCarrier); break;
        default:
            cout << "---------------ѡ�����,������ѡ��----------------" << endl;
        }
        cout << "�Ƿ��˳�ϵͳ��1���˳� 0��������������";
        cin >> OpenStatu;
        if (OpenStatu)break;
    }
}

void SelectAllBook(MYSQL* MidCarrier)
{
    book* ResultBook = new book;
    int KindToFind;
    //����ѡ����
    for (int i = 1;; i++)
    {
        cout << "����Ҫ�鿴����ͼ����嵥��1��ͼ�� 2������ 3���ڿ�����";
        cin >> KindToFind;
        if (KindToFind < 1 || KindToFind>3)
        {
            cout << "---------û������Ҫ��ѯ���͵�ͼ��Ŷ��������ѡ��-----" << endl;
            continue;
        }
        break;
    }
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()�����ǽ����ݸ�ʽ��������ַ���,����Ϊ����������
    //����ѯ���д��SQLSentence��,���Ƹ�ֵ
    switch (KindToFind)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical"); break;
    default:
        cout << "---------û������Ҫ��ѯ���͵�ͼ��Ŷ��������ѡ��-----" << endl;
    }
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() ����һ�������Ľ�����ϸ��ͻ��� 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count���ز�ѯ�������
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------���Ϊ�գ�-------------------" << endl;
        }
    }
    cout << TabLightTop << endl;
    OutBookTabName();
    //mysql_fetch_row() �ӽ��������ȡ����һ��
    //atoi(const char* str) �Ѳ��� str ��ָ����ַ���ת��Ϊһ������
    //  mysql_fetch_row() ƫ������ 0 ��ʼ,����ִ�и�����Զ���ȡ��һ��
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
    //����ѡ����
    for (int i = 1;; i++)
    {
        cout << "��������Ҫ��ѯͼ������ࣨ1��ͼ�� 2������ 3���ڿ�����";
        cin >> KindToFind;
        if (KindToFind < 1 || KindToFind>3)
        {
            cout << "---------û������Ҫ��ѯ���͵�ͼ��Ŷ��������ѡ��-----" << endl;
            continue;
        }
        break;
    }
    cout << "��������Ҫ��ѯ��ͼ���������";
    cin >> BookNameFin;
    //��ѯbook
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()�����ǽ����ݸ�ʽ��������ַ���,����Ϊ����������
    //����ѯ���д��SQLSentence��,���Ƹ�ֵ
    switch (KindToFind)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical"); break;
    default:
        cout << "------���ͼ������ѡ�����е�����밴Ҫ������------" << endl;
    }
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() ����һ�������Ľ�����ϸ��ͻ��� 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count���ز�ѯ�������
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------���Ϊ�գ�-------------------" << endl;
        }
    }
    //mysql_fetch_row() �ӽ��������ȡ����һ��
    //atoi(const char* str) �Ѳ��� str ��ָ����ַ���ת��Ϊһ������
    //  mysql_fetch_row() ƫ������ 0 ��ʼ,����ִ�и�����Զ���ȡ��һ��
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

//SQL������䣺INSERT INTO table_name (��1, ��2,...) VALUES (ֵ1, ֵ2,....)
void InsertBook(MYSQL* MidCarrier)
{
    int KindToInsert;
    //����ѡ����
    for (int i = 1;; i++)
    {
        cout << "����Ҫ�������ͼ�飨1��ͼ�� 2������ 3���ڿ�����";
        cin >> KindToInsert;
        if (KindToInsert < 1 || KindToInsert>3)
        {
            cout << "û������Ҫ��ѯ���͵�ͼ��Ŷ��������ѡ��" << endl;
            continue;
        }
        break;
    }
   
    book BookToInsert;
    //���ڴ��ʱ��
    char* InputDateChar = new char[11];
    cout << "��ʼ��������ͼ����Ϣ" << endl;
    cout << "��ţ�";
    cin >> BookToInsert.BookNumber;
    cout << "������";
    cin >> BookToInsert.BookName;
    cout << "���ߣ�";
    cin >> BookToInsert.Author;
    cout << "�����磺";
    cin >> BookToInsert.Publish;
    cout << "�������ڣ���ʽ��XXXX-XX-XX����";
    cin >> InputDateChar;
    cout << "�۸�";
    cin >> BookToInsert.Price;
    cout << "���";
    cin >> BookToInsert.BookKind;
    cout << "���������";
    cin >> BookToInsert.WholeBook;
    //���ڸ���ӣ�����Ĭ����������ȫ�������������0
    BookToInsert.NumNowBook = BookToInsert.WholeBook;
    BookToInsert.NumBorrow = 0;
    switch (KindToInsert)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "insert into book (���, ����, ����,������,��������,���,���,�������,��ǰ�����,�ѽ������) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook, BookToInsert.NumBorrow); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "insert into journal (���, ����, ����,������,��������,���,���,�������,��ǰ�����,�ѽ������) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook,BookToInsert.NumBorrow); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "insert into periodical (���, ����, ����,������,��������,���,���,�������,��ǰ�����,�ѽ������) values ('%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d)", BookToInsert.BookNumber, BookToInsert.BookName, BookToInsert.Author, BookToInsert.Publish, InputDateChar, BookToInsert.Price, BookToInsert.BookKind, BookToInsert.WholeBook, BookToInsert.NumNowBook, BookToInsert.NumBorrow); break;
    default:
        cout << "���ͼ������ѡ�����е�����밴Ҫ������" << endl;
    }
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "��ӳɹ���" << endl;
    }
    delete[] InputDateChar;
}

//SQL�޸ı���䣺UPDATE ������ SET ������ = ��ֵ WHERE ������ = ĳֵ
//��������ͼ�������ԣ��Ա�ĸ��£��˴�ֻ�漰�������д����ͽ����
//��������������û���¼���ٲ�ѯ���д�����Insert��¼���޴�������ѯ����Ļ���ʱ��
int ReaderOpera(MYSQL* MidCarrier)
{
    int IfKeepGoing = 0;
    int OperaKind;
    int LogOut = 0;
    //�Ƚ��ж���ע����ߵ�¼
    IfKeepGoing=ReaderToLog(MidCarrier);
    if (IfKeepGoing)
    {
        return 1;
    }
    for (int LogSta = 0;; LogSta++)
    {
        cout << "---------------��ѡ����Ҫ���еĲ���---------------" << endl;
        cout << "��1������ 2������ 3�����裩�� ";
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
            cout << "�ݲ�֧����Ҫ���еĲ�����" << endl;
        }
        cout << "�Ƿ��˳���¼(1���˳� 0����������)��";
        cin >> LogOut;
        if (LogOut) break;
    }
    return 0;
}

//�û���¼��ע��
int ReaderToLog(MYSQL* MidCarrier)
{
    int IfLogSucess = 0;
    cout << "---------------��ӭ�������߽��棬��ѡ��-------------" << endl;
    int KindToLog;
    cout << "��ѡ��1����¼ 2��ע�� 3��ע���˻�����  ";
    cin >> KindToLog;
    //�û�֤����
    switch (KindToLog)
    {
    case 1:
        IfLogSucess=SelectReaderInfor(MidCarrier);
        if (IfLogSucess)
        {
            //�����¼ʧ�ܷ���1
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
        cout << "��ĵ�¼�е�����밴Ҫ������" << endl;
        //����������ֵҲ��¼��������
        return 1;
    }
    return 0;
}

//�����û���¼
int SelectReaderInfor(MYSQL* MidCarrier)
{
    //״̬�������������ղ�ѯ״̬���ж�
    int LastJudge = 0;
    char* NumberReader = new char[20];
    char* PassWord1 = new char[20];
    //��ʱ����10�����͵�ַ�ռ�
    readerinfor* ResultOneReader = new readerinfor[10];
    cout << "-------------��ʼ��¼���밴��ʾ������Ϣ-------------" << endl;
    cout << "�û���������֤�ţ���";
    cin >> NumberReader;
    cout << "              ���룺";
    cin >> PassWord1;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from readerinformation");
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    { 
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "---------------���Ϊ�գ���ѡ��ע�ᣡ---------------" << endl;
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
        cout << "---------------------��¼�ɹ�---------------------" << endl;
        //��ʾ��û�л�����
        SelTheReaBook(MidCarrier, NumberReader);
        cout << "-------����ϸ�鿴������Ϣ���������ڻ���Ŷ��-------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneReader;
        return 0;
    }
    else
    {
        cout << "-------------��¼ʧ�ܣ����������û�������---------- " << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneReader;
        return 1;
    }
}

//���ڶ���ע�ᣨ������Ϣ��¼�룩
void InsertReaderInfor(MYSQL* MidCarrier)
{
    readerinfor* NewReader = new readerinfor;
    char *PassWordSec = new char[20];
    cout << "-------------------��ʼע��----------------------" << endl;
    cout << setfill(' ') << setw(10) << "����֤�ţ�";
    cin >> NewReader ->CardNumber;
    cout << setfill(' ') << setw(10) << "������";
    cin >> NewReader->ReaderName;
    cout << setfill(' ') << setw(10) << "�༶��";
    cin >> NewReader->ReaderGrade;
    cout << setfill(' ') << setw(10) << "ѧ�ţ�";
    cin >> NewReader->SchoolNumber;
    cout << setfill(' ') << setw(20) << "�����ݣ�0��ѧ�� 1����ʦ����";
    cin >> NewReader->ReaderLevel;
    for (int i=1;;i++)
    {
        cout << setfill(' ') << setw(10) << "���룺";
        cin >> NewReader->PassWord;
        cout << setfill(' ') << setw(10) << "�ٴ��������룺";
        cin >> PassWordSec;
        if (strcmp(NewReader->PassWord, PassWordSec) == 0)
        {
            break;
        }
        else
        {
            cout << "-------------�������벻һ��,�����������-----------" << endl;
        }
    }
    int NumberBrrow = 0;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "insert into readerinformation (����֤��, ����, �༶,ѧ��,���ߵȼ�,����,���ı���) values ('%s', '%s', '%s', '%s', %d, '%s',%d)", NewReader->CardNumber, NewReader->ReaderName, NewReader->ReaderGrade, NewReader->SchoolNumber, NewReader->ReaderLevel, NewReader->PassWord, NumberBrrow);
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "---------------ע��ɹ�����ǰ����¼��--------------" << endl;
    }
    delete  NewReader;
    delete[] PassWordSec;
}

//���ڶ���ע���˻���DELETE FROM ������ WHERE ������ = ֵ
void DeleteReaderInfor(MYSQL* MidCarrier)
{
    int IfKeepDelete;
    readerinfor* CancelReader = new readerinfor;
    cout << "-------------------��ʼע��----------------------" << endl;
    cout << setfill(' ') << setw(10) << "����֤�ţ�";
    cin >> CancelReader->CardNumber;
    cout << setfill(' ') << setw(10) << "���룺";
    cin >> CancelReader->PassWord;
    int NumberBrrow = 0;
    cout << "ȷ��Ҫע���˻��𣿣�1��ȷ�� 0����������";
    cin >> IfKeepDelete;
    if (IfKeepDelete)
    {
        //+		SQLSentence	0x00007ff7ac6b67f0 "select * from brrowrecord where ����֤��='2020121207' AND ��������=NULL"	char[1024]

        mysql_query(MidCarrier, "SET NAMES 'GBK'");
        sprintf_s(SQLSentence, 1024, "select * from brrowrecord where ����֤��='%s' AND �������� is NULL", CancelReader->CardNumber);
        if (mysql_query(MidCarrier, SQLSentence) == 0)
        {
            SelectResult = mysql_store_result(MidCarrier);
            if (SelectResult->row_count >= 1)
            {
                cout << "����ǰ��" << SelectResult->row_count << "����δ�����뻹�����ע���˻���" << endl;
            }
            else
            {
                cout << "����ǰ��δ�黹�飬����ע���˻���" << endl;
                sprintf_s(SQLSentence, 1024, "delete from readerinformation where ����֤��='%s' AND ����='%s' ", CancelReader->CardNumber, CancelReader->PassWord);
                if (mysql_query(MidCarrier, SQLSentence) == 0)
                {
                    cout << "-----------------�˻�ע���ɹ���------------------" << endl;
                }
                else
                {
                    cout << "----------ע��ʧ�ܣ��������֤�ź����룡---------" << endl;
                }
            }
        }
    }
    delete  CancelReader;
}

//��ѯ���û���Ҫ�����飬,�û���¼���Զ�ִ�У��г����иõ�¼�û����ĵ����е��飬���������û�����
void SelTheReaBook(MYSQL* MidCarrier, char* NumberReader)
{
    brrowrecord* ResultSelectRecord = new brrowrecord[30];  
    cout << "------------------��ǰ�����ĵ���------------------" << endl;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()�����ǽ����ݸ�ʽ��������ַ���,����Ϊ����������
    //����ѯ���д��SQLSentence��,���Ƹ�ֵ
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord where �������� is NULL");
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() ����һ�������Ľ�����ϸ��ͻ��� 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count���ز�ѯ�������
        if (SelectResult->row_count <= 0)
        {
            cout << "���ļ�¼��Ϊ�գ�" << endl;
        }
    }
    //mysql_fetch_row() �ӽ��������ȡ����һ��
    //atoi(const char* str) �Ѳ��� str ��ָ����ַ���ת��Ϊһ������
    //  mysql_fetch_row() ƫ������ 0 ��ʼ,����ִ�и�����Զ���ȡ��һ��
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
            //��ʱ�Ȳ����ʷ���
        }
        if (strcmp(ResultSelectRecord->CardNumber, NumberReader)==0)
        {
            cout << TabReaRecLightMid << endl;
            cout << "�� "  << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " �� " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-"<<setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " �� " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " ��" << endl;
        }
    }
    cout << TabReaRecLightButton << endl;
    delete[] ResultSelectRecord;
    mysql_free_result(SelectResult);
}

//������ļ�¼�����飬��ӽ��ļ�¼��������������û����ı���
int BrrowBook(MYSQL* MidCarrier)
{
    int IfToBrrow;
    int NumTheReaBrr;
    int KindOpera;
    int NumberNow=0;
    int NumberBrrow=0;
    int ReaderBrrowNow = 1;
    cout << "�Ƿ�Ҫ�鿴ȫ��ͼ��Ŀ¼��1���鿴 0��ֱ������ָ��ͼ�飩��";
    cin >> KindOpera;
    if (KindOpera)
    {
        SelectAllBook(MidCarrier);
    }
    //�Ƚ���ͼ��Ĳ�ѯ
    cout << "-------------------����ͼ���ѯ------------------" << endl;
    if (SelectOneBook(MidCarrier) <= 0)
    {
        cout << "---------���ѯ��ͼ����ȫ�����,��������!----------" << endl;
    }
    else
    {
        cout << "-----------���ѯͼ�黹������,���Խ���!-----------" << endl;
        cout << "�Ƿ���н��ģ�1������ 0�������ģ���";
        cin >> IfToBrrow;
        if (IfToBrrow)
        {
            //���ڽ��ĳɹ���ͼ����Ϣ����޸Ĳ���
            int KindBook;
            int ReaderLevel;
            brrowrecord *RecordToInsert=new brrowrecord;
            //���ڴ��ʱ��
            char* InputBrrowDate = new char[11];
            cout << "---------------��ʼ����ͼ�������Ϣ---------------" << endl;
            cout << "��������Ҫ����ͼ������ࣨ1��ͼ�� 2������ 3���ڿ�����";
            cin >> KindBook;
            cout << "ͼ���ţ�";
            cin >> RecordToInsert->BookNumber;
            cout << "������";
            cin >> RecordToInsert->BookName;
            cout << "��";
            cin >> RecordToInsert->Price;
            cout << "����֤�ţ�";
            cin >> RecordToInsert->CardNumber;
            cout << "�������ڣ���ʽ��XXXX-XX-XX����";
            cin >> InputBrrowDate;
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            sprintf_s(SQLSentence, 1024, "select * from brrowrecord where ����֤��='%s' AND �������� is NULL", RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count >= 1)
                {
                    cout << "--------------����ǰ���ڽ���" << SelectResult->row_count << "��ͼ�飡-------------" << endl;
                }
                else
                {
                    cout << "------------����ǰû�����ڽ��ĵ�ͼ�飡------------" << endl;
                }
            }
            NumTheReaBrr = SelectResult->row_count;

            sprintf_s(SQLSentence, 1024, "select ���ߵȼ� from readerinformation where ����֤��='%s' ", RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            { 
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count <= 0)
                {
                    cout << "--------------------�޸��û���-------------------" << endl;
                }
                else
                {
                    OneRowResult = mysql_fetch_row(SelectResult);
                    ReaderLevel = atoi(OneRowResult[0]);
                    if (ReaderLevel)
                    {
                        cout << "----------------����Ϊ���Ϊ��ʦ��----------------"<< endl;
                        if (NumTheReaBrr >= 5)
                        {
                            cout << "���Ľ��ı����Ѵﵽ���ޣ�5����,��黹ͼ����ڽ��ģ�" << endl;
                            return 1;
                        }
                        else
                        {
                            cout << "���Ľ��ı���δ�ﵽ���ޣ�5���������Խ��н��ģ�" << endl;
                        }
                    }
                    else
                    {
                        cout << "----------------����Ϊ���Ϊѧ����----------------" << endl;
                        if (NumTheReaBrr >= 3)
                        {
                            cout << "���Ľ��ı����Ѵﵽ���ޣ�3����,��黹ͼ����ٽ��ģ�" << endl;
                            return 1;
                        }
                        else
                        {
                            cout << "���Ľ��ı���δ�ﵽ���ޣ�3���������Խ��н��ģ�" << endl;
                        }
                    }
                }
            }
            Date DateBrrow = CharChangeDate(InputBrrowDate);
            //�Զ����㻹��ʱ��Ϊһ���£�30�죩��
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
            //���ڸս��ģ����Է����ʼֵΪ0
            RecordToInsert->Fine = 0;
            sprintf_s(SQLSentence, 1024, "insert into brrowrecord (���, ����, ���,����֤��,��������,��������,������) values ('%s', '%s', %d, '%s', '%s', '%d-%d-%d', %d)", RecordToInsert->BookNumber, RecordToInsert->BookName, RecordToInsert->Price, RecordToInsert->CardNumber, InputBrrowDate, ReturnDate.year, ReturnDate.month, ReturnDate.day,RecordToInsert->Fine);
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "���ĳɹ���������죬����30���ڻ���Ŷ�����򽫻ᷣ�" << endl;
            }
            mysql_query(MidCarrier, "SET NAMES 'GBK'");
            switch (KindBook)
            {
            case 1:
                sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from book where ����='%s'", RecordToInsert->BookName); break;
            case 2:
                sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from journal where ����='%s'", RecordToInsert->BookName); break;
            case 3:
                sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from periodical where ����='%s'", RecordToInsert->BookName); break;
            default:
                cout << "��ѡ���ͼ�������е�����밴Ҫ������" << endl;
            }
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                SelectResult = mysql_store_result(MidCarrier);
                if (SelectResult->row_count <= 0)
                {
                    cout << "--------------------���Ϊ�գ�-------------------" << endl;
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
                sprintf_s(SQLSentence, 1024, "update book set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow,RecordToInsert->BookName); break;
            case 2:
                sprintf_s(SQLSentence, 1024, "update journal set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow, RecordToInsert->BookName); break;
            case 3:
                sprintf_s(SQLSentence, 1024, "update periodical set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow, RecordToInsert->BookName); break;
            default:
                cout << "��ѡ���ͼ�������е�����밴Ҫ������" << endl;
            }
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "------------------ͼ�����³ɹ�-----------------" << endl;
            }

            sprintf_s(SQLSentence, 1024, "select * from readerinformation where ����֤��='%s'", RecordToInsert->CardNumber);
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
            sprintf_s(SQLSentence, 1024, "update readerinformation set ���ı���=%d where ����֤��='%s'", ReaderBrrowNow, RecordToInsert->CardNumber);
            if (mysql_query(MidCarrier, SQLSentence) == 0)
            {
                cout << "---------------���߽��ı������³ɹ�--------------" << endl;
            }
            delete RecordToInsert;
            delete[] InputBrrowDate;
            mysql_free_result(SelectResult);
        }
        else
        {
            cout << "-----------------������һ������!-----------------" << endl;
        }
    }
    return 1;
}

//���飬���»���ʱ�䡢�������
void ReturnBook(MYSQL* MidCarrier)
{
    cout << "---------------------�������---------------------" << endl;
    int FineEachDay = 1;
    int BookKindReturn;
    int NumberNow = 0;
    int NumberBrrow = 0;
    brrowrecord* PapreToReturn = new brrowrecord;
    char* DateToReturn = new char[11];
    char* BrrowDate = new char[11];
    cout << "��ѡ�����������ࣨ1��ͼ�� 2������ 3���ڿ� ����";
    cin >> BookKindReturn;
    cout << "������";
    cin >> PapreToReturn->BookName;
    cout << "����֤�ţ�";
    cin >> PapreToReturn->CardNumber;
    cout << "����ʱ�䣨��ʽ��XXXX-XX-XX����";
    cin >> BrrowDate;
    cout << "�����뵱ǰ��ʱ�䣨��ʽ��XXXX-XX-XX����";
    cin >> DateToReturn; 
    Date DateReturn = CharChangeDate(DateToReturn);
    Date DateBrrow = CharChangeDate(BrrowDate);
    //Ϊ�˷�����㣬�涨ÿ����Ȼ��Ϊ30��,ÿ��360��
    if ((DateReturn.year * 360 + DateReturn.month * 30 + DateReturn.day - DateBrrow.year * 360 - DateBrrow.month * 30 - DateBrrow.day - 30) > 0)
    {
        PapreToReturn->Fine = DateReturn.year * 360 + DateReturn.month * 30 + DateReturn.day - DateBrrow.year * 360 - DateBrrow.month * 30 - DateBrrow.day - 30;
        cout << "��֧��" << PapreToReturn->Fine << "Ԫ���" << endl;
    }
    else
    {
        cout << "�涨ʱ���ڻ��飬����֧������!" << endl;
        PapreToReturn->Fine = 0;
    }
    PapreToReturn->Fine = PapreToReturn->Fine * FineEachDay;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "update brrowrecord set ������=%d,��������='%s' where ����='%s' AND ����֤��='%s' AND ��������='%s'", PapreToReturn->Fine, DateToReturn, PapreToReturn->BookName, PapreToReturn->CardNumber, BrrowDate);
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "--------------------����ɹ���-------------------" << endl;
        switch (BookKindReturn)
        {
        case 1:
            sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from book where ����='%s'", PapreToReturn->BookName); break;
        case 2:
            sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from journal where ����='%s'", PapreToReturn->BookName); break;
        case 3:
            sprintf_s(SQLSentence, 1024, "select ��ǰ�����,�ѽ������ from periodical where ����='%s'", PapreToReturn->BookName); break;
        default:
            cout << "��ѡ���ͼ�������е�����밴Ҫ������" << endl;
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
            sprintf_s(SQLSentence, 1024, "update book set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        case 2:
            sprintf_s(SQLSentence, 1024, "update journal set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        case 3:
            sprintf_s(SQLSentence, 1024, "update periodical set ��ǰ�����=%d,�ѽ������=%d where ����='%s'", NumberNow, NumberBrrow, PapreToReturn->BookName); break;
        default:
            cout << "��ѡ���ͼ�������е�����밴Ҫ������" << endl;
        }
        if (mysql_query(MidCarrier, SQLSentence) == 0)
        {
            cout << "------------------ͼ�����³ɹ�-----------------" << endl;
        }
    }
    delete PapreToReturn;
    delete[] DateToReturn;
    delete[] BrrowDate;
}

//���ڲ������˴�Ĭ������30�죬ͨ��������ʱ��͵���ʱ�����һ���£�Ĭ��30�죩ʵ��
void ReNewDate(MYSQL* MidCarrier)
{
    cout << "---------------------���ڲ���---------------------" << endl;
    brrowrecord* PapreToRenew = new brrowrecord;
    char* BrrowDate = new char[11];
    cout << "������";
    cin >> PapreToRenew->BookName;
    cout << "����֤�ţ�";
    cin >> PapreToRenew->BookNumber;
    cout << "����ʱ�䣨��ʽ��XXXX-XX-XX����";
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
    //+		SQLSentence	0x00007ff78b3817f0 "update brrowrecord set ��������='2000-10-5',��������='2000-11-5' where ����='����ʷ��' AND ����֤��='2020120106' AND ��������='2000-09-05'"	char[1024]


    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "update brrowrecord set ��������='%d-%d-%d',��������='%d-%d-%d' where ����='%s' AND ����֤��='%s' AND ��������='%s'", DateBrrow.year, DateBrrow.month, DateBrrow.day, DateReturn.year, DateReturn.month, DateReturn.day, PapreToRenew->BookName, PapreToRenew->BookNumber, BrrowDate);

    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        cout << "--------------------���ڳɹ���-------------------" << endl;
    }
    delete PapreToRenew;
    delete[] BrrowDate;
}

int AdminiOpera(MYSQL* MidCarrier)
{
    int IfLogSucess = 0;
    //����Ա��¼
    IfLogSucess=SelectAdminInfor(MidCarrier);
    if (IfLogSucess)
    {
        return 0;
    }
    int KindToOpera;
    int LogOutSta;
    for (int LogSta = 0;; LogSta++)
    {
        cout << "---------------��ѡ����Ҫ���еĲ���---------------" << endl;
        cout << "��1��������� 2���鿴ȫ��ͼ��״̬ 3���鿴�����Ķ����� 4���鿴ͼ��������� 5���鿴ȫ�����ļ�¼ 6���鿴����δ�黹ͼ�飩�� ";
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
            cout << "-------------�ݲ�֧����Ҫ���еĲ�����-------------" << endl;
        }
        cout << "�Ƿ��˳���¼(1���˳� 0����������)��";
        cin >> LogOutSta;
        if (LogOutSta) break;
    }
    return 0;
}

//��ѯȫ���Ľ��ļ�¼,����Աʹ��
void SelectRecord(MYSQL* MidCarrier)
{
    brrowrecord* ResultSelectRecord = new brrowrecord[10];
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()�����ǽ����ݸ�ʽ��������ַ���,����Ϊ����������
    //����ѯ���д��SQLSentence��,���Ƹ�ֵ
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord ");
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() ����һ�������Ľ�����ϸ��ͻ��� 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count���ز�ѯ�������
        if (SelectResult->row_count <= 0)
        {
            cout << "----------------���ļ�¼��Ϊ�գ�----------------" << endl;
        }
    }
    //mysql_fetch_row() �ӽ��������ȡ����һ��
    //atoi(const char* str) �Ѳ��� str ��ָ����ַ���ת��Ϊһ������
    //  mysql_fetch_row() ƫ������ 0 ��ʼ,����ִ�и�����Զ���ȡ��һ��
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
            //��ʱ�Ȳ����ʷ���
        }
        cout << TabRecordLightMid << endl;
        cout << "�� " << setfill(' ') << setw(20) << ResultSelectRecord->BookNumber << " �� " << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " �� " << setfill(' ') << setw(4) << ResultSelectRecord->Price << " �� " << setfill(' ') << setw(20) << ResultSelectRecord->CardNumber << " �� " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " �� " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " ��" << endl;
    }
    cout << TabRecordLightButton << endl;
    delete[] ResultSelectRecord;
    mysql_free_result(SelectResult);
}

//�鿴���߽�������
void RankListReader(MYSQL* MidCarrier)
{
    //������ð��ս��ı�����������ֻ��˳���������
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from readerinformation order by ���ı��� desc");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    { 
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "----------------���ļ�¼��Ϊ�գ�----------------" << endl;
        }
    }
    cout << TabRankListTop << endl;
    cout << "�� " << setfill(' ') << setw(20) << "����" << " �� " << setfill(' ') << setw(10) << "���ı���" << " ��" << endl;
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        readerinfor* RankListReader = new readerinfor;
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(RankListReader->ReaderName, 30, OneRowResult[1]);
            RankListReader->NumberBrrow = atoi(OneRowResult[6]);
        }
        cout << TabRankListMid << endl;
        cout << "�� " << setfill(' ') << setw(20) << RankListReader->ReaderName << " �� " << setfill(' ') << setw(10) << RankListReader->NumberBrrow << " ��" << endl;
        delete RankListReader;
    }
    cout << TabRankListButton << endl;
    mysql_free_result(SelectResult);
}

//�鿴������ͼ������
void RankListBook(MYSQL* MidCarrier) 
{
    int KindBook;
    cout << "����Ҫ�鿴����ͼ��Ľ������У�1��ͼ�� 2������ 3���ڿ�����";
    cin >> KindBook;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    //sprintf_s()�����ǽ����ݸ�ʽ��������ַ���,����Ϊ����������
    //����ѯ���д��SQLSentence��,���Ƹ�ֵ
    switch (KindBook)
    {
    case 1:
        sprintf_s(SQLSentence, 1024, "select * from book order by �ѽ������ desc"); break;
    case 2:
        sprintf_s(SQLSentence, 1024, "select * from journal order by �ѽ������ desc"); break;
    case 3:
        sprintf_s(SQLSentence, 1024, "select * from periodical order by �ѽ������ desc"); break;
    default:
        cout << "���ͼ������ѡ�����е�����밴Ҫ������" << endl;
    }
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        //mysql_store_result() ����һ�������Ľ�����ϸ��ͻ��� 
        SelectResult = mysql_store_result(MidCarrier);
        //row_count���ز�ѯ�������
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------------���Ϊ�գ�-------------------" << endl;
        }
    }
    cout << TabRankListTop << endl;
    cout << "�� " << setfill(' ') << setw(20) << "����" << " �� " << setfill(' ') << setw(10) << "�ѽ������" << " ��" << endl;
    for (int i = 0; i < SelectResult->row_count; i++)
    {
        book* RankListBook = new book;
        if (OneRowResult = mysql_fetch_row(SelectResult))
        {
            sprintf_s(RankListBook->BookName, 30, OneRowResult[1]);
            RankListBook->NumBorrow = atoi(OneRowResult[9]);
        }
        cout << TabRankListMid << endl;
        cout << "�� "  << setfill(' ') << setw(20) << RankListBook->BookName << " �� " << setfill(' ') << setw(10) << RankListBook->NumBorrow << " ��" << endl;
        delete RankListBook;
    }
    cout << TabRankListButton << endl;
    mysql_free_result(SelectResult);

}

//�鿴�ѽ��δ����ͼ����Ϣ
void BookHadBrrow(MYSQL* MidCarrier)
{
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from brrowrecord ");
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "-----------------���ļ�¼��Ϊ�գ�----------------" << endl;
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
                cout << "�� " << setfill(' ') << setw(20) << ResultSelectRecord->BookNumber << " �� " << setfill(' ') << setw(20) << ResultSelectRecord->BookName << " �� " << setfill(' ') << setw(4) << ResultSelectRecord->Price << " �� " << setfill(' ') << setw(20) << ResultSelectRecord->CardNumber << " �� " << setfill('0') << setw(4) << ResultSelectRecord->BorrowDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->BorrowDate.day << " �� " << setfill('0') << setw(4) << ResultSelectRecord->ReturnDate.year << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.month << "-" << setfill('0') << setw(2) << ResultSelectRecord->ReturnDate.day << " ��" << endl;
            }
            delete ResultSelectRecord;
        }
    }
    cout << TabRecordLightButton << endl;
    mysql_free_result(SelectResult);
}

//����Ա��½
int SelectAdminInfor(MYSQL* MidCarrier)
{
    //״̬�������������ղ�ѯ״̬���ж�
    int LastJudge = 0;
    char* NumberReader = new char[20];
    char* PassWord1 = new char[20];
    //��ʱ����10�����͵�ַ�ռ�
    admini* ResultOneAdmin = new admini[10];
    cout << "-------------------����Ա��¼����------------------" << endl;
    cout << "�û�����֤���ţ���";
    cin >> NumberReader;
    cout << "            ���룺";
    cin >> PassWord1;
    mysql_query(MidCarrier, "SET NAMES 'GBK'");
    sprintf_s(SQLSentence, 1024, "select * from administrator");
    //mysql_query() ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ�� 
    //ִ�в�ѯ���,��������洢��SelectResult��
    if (mysql_query(MidCarrier, SQLSentence) == 0)
    {
        SelectResult = mysql_store_result(MidCarrier);
        if (SelectResult->row_count <= 0)
        {
            cout << "--------------���Ϊ�գ���ѡ��ע�ᣡ--------------" << endl;
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
        cout << "--------------------��¼�ɹ���-------------------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneAdmin;
        mysql_free_result(SelectResult);
        return 0;
    }
    else
    {
        cout << "-------------��¼ʧ�ܣ��û������������-----------" << endl;
        delete[] NumberReader;
        delete[] PassWord1;
        delete[] ResultOneAdmin;
        mysql_free_result(SelectResult);
        //�����¼ʧ��ֱ�Ӳ���ִ����������
        return 1;
    }
}

//�����ַ�����date����ת��
//ASCII 0=48
date CharChangeDate(char* Origin)
{
    date DateNow;
    DateNow.year = (int(Origin[0])-48)*1000 + (int(Origin[1])-48) * 100 +(int (Origin[2])-48) * 10 + (int(Origin[3])-48);
    DateNow.month = (int(Origin[5])-48) * 10 + int(Origin[6])-48;
    DateNow.day = (int(Origin[8])-48) * 10 + int(Origin[9])-48;
    return DateNow;
}

//���ͼ���ѯ���
void OutPutBookInfor(book* ResultBook)
{
    cout << "�� " << setfill(' ') << setw(20) << ResultBook->BookNumber << " �� " << setfill(' ') << setw(20) << ResultBook->BookName << " �� " << setfill(' ') << setw(20) << ResultBook->Author << " �� " << setfill(' ') << setw(20) << ResultBook->Publish << " �� " << setfill(' ') << setw(4) << ResultBook->PublishDate.year << "-" << setfill('0') << setw(2) << ResultBook->PublishDate.month << "-" << setfill('0') << setw(2) << ResultBook->PublishDate.day << " �� " << setfill(' ') << setw(4) << ResultBook->Price << " �� " << setfill(' ') << setw(4) << ResultBook->BookKind << " �� " << setfill(' ') << setw(8) << ResultBook->WholeBook << " �� " << setfill(' ') << setw(10) << ResultBook->NumNowBook << " �� " << setfill(' ') << setw(10) << ResultBook->NumBorrow << " ��" << endl;

}
