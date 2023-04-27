#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAX_BOOKS 100 // 最多读取的书籍数量

char current_user[20];

struct book
{
    char ISBN[20];
    char title[100];
    char author[100];
    char publisher[100];
    int price;
    char status[100];
};

enum SortOrder
{
    ASCENDING,
    DESCENDING
};

enum SortField
{
    ISBN,
    TITLE,
    AUTHOR,
    PUBLISHER,
    PRICE,
    STATUS
};

/*
int main()
{
    int status_code = book_search();
    printf("status_code = %d", status_code);
}*/

int main()
{
    int status_code = 1;
    for (; status_code == 1;)
    {
        status_code = main_interface();
        if (status_code == 0)
        {
            status_code = admin_login();
            for (; status_code == 0;)
            {
                status_code = book_add();
            }
        }
        else if (status_code == 1)
        {
            status_code = user_login();
            if (status_code == 2)
            {
                printf("The number of borrowed books has reached the upper limit\n Please return the book before borrowing the book\n");
                status_code = 1;
            }
            for (; status_code == 0;)
            {
                status_code = book_borrow();
            }
        }
        else if (status_code == 2)
        {
            status_code = 0;

            for (; status_code == 0;)
            {
                status_code = book_return();
            }
        }
        else if (status_code == 3)
        {
            status_code = 0;
            for (; status_code == 0;)
            {
                status_code = book_search();
            }
        }
        else if (status_code == 4)
        {
            status_code = 0;
            for (; status_code == 0;)
            {
                status_code = book_compensation();
            }
        }
        else if (status_code == 5)
        {
            status_code = 0;
            for (; status_code == 0;)
            {
                status_code = query_record();
            }
        }
        else
            return 0;
    }
}

int main_interface()
{
    printf("0,add book\n1,borrow book\n2,return book\n3,search book\n4,compensation book\n5,query borrowing book record\n");
    int a;
    scanf("%d", &a);
    return a;
}

int book_add()
{
    char ISBN[20];
    puts("Enter the ISBN");
    char str[100] = {0};
    do
    {
        gets(ISBN);
    } while (!ISBN[0]);
    int amount;
    printf("Enter the amount\n");
    scanf("%d", &amount);

    // 检测书本是否存在
    int status_code;
    char path[100];
    strcpy(path, "./data/book/");
    strcat(path, ISBN);
    status_code = 0;
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        status_code = 1;

    fclose(fp);

    if (status_code == 0)
    {
        // 录入已有的书
        FILE *file;
        file = fopen(path, "a");
        for (int i = 1; i <= amount; i++)
        {
            fputs("1", file);
        }
        fclose(file);
        printf("0,Continue adding books\n1,Back to Home page\n");
        scanf("%d", &status_code);
        if (status_code == 0)
            return 0; // 继续加书
        return 1;     // 返回主界面
    }

    char title[100];
    char author[100];
    char publisher[100];
    printf("Enter the title of the book\n");

    do
    {
        gets(title);
    } while (!title[0]);

    printf("Enter the author name\n");
    do
    {
        gets(author);
    } while (!author[0]);

    printf("Enter publisher name\n");
    do
    {
        gets(publisher);
    } while (!publisher[0]);
    double x;
    int price;
    printf("Enter the price\n");
    scanf("%lf", &x);
    price = x * 100;
    // 录入新书
    FILE *fp1 = fopen(path, "w");
    fprintf(fp1, title);
    fprintf(fp1, "\n");
    fprintf(fp1, author);
    fprintf(fp1, "\n");
    fprintf(fp1, publisher);
    fprintf(fp1, "\n");
    fprintf(fp1, "%d", price);
    fprintf(fp1, "\n");
    for (int i = 1; i <= amount; i++)
    {
        fputs("1", fp1);
    }
    fclose(fp);

    printf("0,Continue adding books\n1,Back to Home page\n");
    scanf("%d", &status_code);
    if (status_code == 0)
        return 0; // 继续加书
    return 1;     // 返回主界面
}

int admin_login()
{
    char name[20];
    char password[20];
    char true_password[20];
    char path[30];
    int status_code = 0;
    for (; status_code == 0;)
    {
        printf("Enter an administrator username\n");
        do
        {
            gets(name);
        } while (!name[0]);
        strcpy(path, "./data/admin/");
        strcat(path, name);
        FILE *fp = fopen(path, "r");
        if (fp != NULL)
            status_code = 1;
        else
        {
            fclose(fp);
            printf("Username does not exist\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
            status_code *= 2;
            // 0，重试
            // 1，下一步
            // 2，返回主界面
        }
    }
    if (status_code == 2)
        return 1; // 返回主界面
    status_code = 0;
    for (; status_code == 0;)
    {
        FILE *fp = fopen(path, "r");
        fgets(true_password, 20, fp);
        printf("Enter password\n");
        scanf("%s", &password);
        if (strcmp(true_password, password) == 0)
            return 0; // 通过
        else
        {
            printf("Wrong password\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
        }
        if (status_code == 1)
            return 1; // 返回主界面
    }
}

int book_return()
{
    int status_code;
    char ISBN[20];
    printf("Enter the ISBN\n");
    do
    {
        gets(ISBN);
    } while (!ISBN[0]);

    char path[40];
    strcpy(path, "./data/book/");
    strcat(path, ISBN);
    FILE *fp = fopen(path, "r+");
    if (fp != NULL)
        status_code = 1;
    else
    {
        fclose(fp);
        printf("Book does not exist\n0,Retry\n1,Back to Home page\n");
        scanf("%d", &status_code);
        status_code *= 2;
        // 0，重试
        // 1，下一步
        // 2，返回主界面
    }
    if (status_code == 2)
        return 1; // 返回主界面

    int book_number;
    printf("Enter book number\n");
    scanf("%d", &book_number);

    // 修改文件的第 book_number 个字符（因为中途修改了文件结构，该部分代码被放弃）
    /*fseek (fp,book_number-1,SEEK_SET);
    putc ('c',fp);
    fclose (fp);*/

    // 修改文件第 5 行的第 book_number 个字符为“１”
    char buffer[100];
    int count = 0;
    while (fgets(buffer, 100, fp) != NULL)
    {
        count++;
        if (count == 5)
        {
            fseek(fp, -strlen(buffer), SEEK_CUR);
            for (int i = 0; i < book_number - 1; i++)
                fputc(buffer[i], fp);
            fputc('1', fp);
            for (int i = book_number; i < strlen(buffer); i++)
                fputc(buffer[i], fp);
            break;
        }
    }
    // 遍历 ./data/student/ 文件夹中的文件，删除这条借书记录。
    char ISBNAND[40];
    strcpy(ISBNAND, ISBN);
    strcat(ISBNAND, "&");
    char book_numberc[10];
    itoa(book_number, book_numberc, 10);
    strcat(ISBNAND, book_numberc);
    strcat(ISBNAND, "&");
    remove_lines_start_with_string("./data/student/", ISBNAND);

    fclose(fp);
    printf("0,Continue returning books\n1,Back to Home page\n");
    scanf("%d", &status_code);
    if (status_code == 0)
        return 0; // 继续还书
    return 1;     // 返回主界面
}

// Windows环境下删除指定文件夹中所有文件中以指定字符串开头的行的函数。
// 此函数假定文件夹中的所有文件都是UTF-8编码的。
// 这个函数首先遍历指定文件夹中的所有文件，
// 然后逐个打开文件，读取文件内容并删除以指定字符串开头的行。
// 删除后，将临时文件重命名为原始文件名，并删除原始文件。
void remove_lines_start_with_string(const char *folder_path, const char *prefix)
{
    WIN32_FIND_DATAA file_data;
    HANDLE handle;
    char file_path[MAX_PATH];
    char temp_path[MAX_PATH];
    FILE *in_file, *out_file;
    char line[1024];
    int prefix_len = strlen(prefix);

    // 构造查询文件的路径
    snprintf(file_path, sizeof(file_path), "%s\\*", folder_path);
    handle = FindFirstFileA(file_path, &file_data);
    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open folder: %s\n", folder_path);
        return;
    }

    do
    {
        // 忽略文件夹以及"."和".."目录
        if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ||
            strcmp(file_data.cFileName, ".") == 0 ||
            strcmp(file_data.cFileName, "..") == 0)
        {
            continue;
        }

        // 构造输入文件和输出文件的路径
        snprintf(file_path, sizeof(file_path), "%s\\%s", folder_path, file_data.cFileName);
        snprintf(temp_path, sizeof(temp_path), "%s.temp", file_path);

        // 打开文件
        in_file = fopen(file_path, "r");
        if (in_file == NULL)
        {
            printf("Unable to open file: %s\n", file_path);
            continue;
        }

        out_file = fopen(temp_path, "w");
        if (out_file == NULL)
        {
            printf("Unable to write to file: %s\n", temp_path);
            fclose(in_file);
            continue;
        }

        // 逐行读取文件内容并删除以指定字符串开头的行
        while (fgets(line, sizeof(line), in_file) != NULL)
        {
            if (strncmp(line, prefix, prefix_len) != 0)
            {
                fputs(line, out_file);
            }
        }

        // 关闭文件
        fclose(in_file);
        fclose(out_file);

        // 删除原始文件并将临时文件重命名为原始文件名
        if (remove(file_path) != 0)
        {
            printf("Unable to delete file: %s\n", file_path);
            continue;
        }

        if (rename(temp_path, file_path) != 0)
        {
            printf("Unable to rename file: %s\n", file_path);
            continue;
        }

    } while (FindNextFileA(handle, &file_data) != 0);

    // 关闭文件句柄
    FindClose(handle);
}

// 借书
int book_borrow()
{
    int status_code = 0;

    for (; status_code == 0;)
    {
        char ISBN[20];
        printf("Enter the ISBN\n");
        do
        {
            gets(ISBN);
        } while (!ISBN[0]);
        int number;
        printf("Enter the number\n");
        scanf("%d", &number);

        // 检测书本是否存在
        char path[100];
        strcpy(path, "./data/book/");
        strcat(path, ISBN);
        check_char(status_code, path, number, '1');
        if (status_code == -1)
        {
            printf("The book does not exist\n");
            continue;
        }
        else if (status_code == 1)
        {
            printf("The book has been lent\n");
            continue;
        }
        modify_char(path, 5, number, '0'); // 修改书本状态

        // 增加借书记录
        strcpy(path, "./data/student/");
        strcat(path, current_user);
        FILE *fp = fopen(path, "a");
        char date_str[11];
        get_date(date_str);
        fprintf(fp, "%s&%d&%s&\n", ISBN, number, date_str);
        fclose(fp);
        printf("0,Continue borrowing books\n1,Back to Home page\n");
        scanf("%d", &status_code);
        if (status_code == 0)
            return 0; // 继续加书
        return 1;     // 返回主界面
    }
}

// 登录并检测是否能借更多的书
int user_login()
{
    char name[20];
    char password[20];
    char true_password[20];
    char path[30];
    int status_code = 0;
    for (; status_code == 0;)
    {
        printf("Enter your student ID\n");
        do
        {
            gets(name);
        } while (!name[0]);
        strcpy(path, "./data/student/");
        strcat(path, name);
        FILE *fp = fopen(path, "r");
        if (fp != NULL)
        {
            status_code = 1;
            fclose(fp);
        }
        else
        {
            fclose(fp);
            printf("Username does not exist\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
            status_code *= 2;
            // 0，重试
            // 1，下一步
            // 2，返回主界面
        }
    }

    if (status_code == 2)
        return 1; // 返回主界面
    status_code = 0;
    for (; status_code == 0;)
    {
        printf("Enter password\n");
        scanf("%s", &password);
        get_line(true_password, path, 2);
        strcat(password, "\n");
        if (strcmp(true_password, password) == 0)
            if (count_lines(path) <= 5) // 上限为 3 本
            {
                strcpy(current_user, name);
                return 0; // 通过
            }
            else
                return 2; // 已借满，不能继续借书
        else
        {
            printf("Wrong password\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
        }
        if (status_code == 1)
            return 1; // 返回主界面
    }
}

// 检测目标文件行数，可以用于计算学生借阅的书籍数量
int count_lines(const char *filename)
{
    FILE *file = fopen(filename, "r"); // 打开文件以读取模式
    if (file == NULL)
    {
        // 文件打开失败
        return -1;
    }

    int line_count = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line_count++;
        }
    }

    fclose(file); // 关闭文件
    return line_count;
}

// 检测指定文件的第 n 个字符是否为 x
// 函数会将文件指针定位到文件的第 n 个字符处，并读取该字符
// 如果读取到的字符等于 x，则函数返回 1，否则返回 0，有错误返回 -1
// 可用于检测书本借出情况。
void check_char(int status_code, const char *filename, int n, char x)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: file pointer is NULL\n");
        status_code = -1;
    }
    if (n <= 0)
    {
        printf("Error: n must be a positive integer\n");
        status_code = -1;
    }
    fseek(file, n - 1, SEEK_SET);
    char c = fgetc(file);
    if (c == EOF)
    {
        printf("Error: reached end of file before reaching position %d\n", n);
        status_code = -1;
    }
    if (c == x)
    {
        status_code = 1;
    }
    else
    {
        status_code = 0;
    }
}

// 修改指定路径的文件的第 n 个字符 为 x（因为中途修改了文件结构，该部分代码被放弃）
/*void modify_char(const char *path, int n, char x)
{
    FILE *fp = fopen(path, "r+"); // 以读写方式打开文件
    if (fp == NULL)
    {
        printf("Failed to open file.\n");
        return;
    }
    fseek(fp, n - 1, SEEK_SET); // 将文件指针移动到第 n 个字符
    fputc(x, fp);               // 将第 n 个字符修改为 x
    fclose(fp);                 // 关闭文件
}*/

// 修改指定路径的文件的第 n 行第 m 个字符为 x
void modify_char(const char *filePath, int n, int m, char x)
{
    FILE *file = fopen(filePath, "r+");
    if (file == NULL)
    {
        printf("Failed to open file %s.\n", filePath);
        return;
    }

    char ch;
    int line = 1, col = 1;
    while ((ch = fgetc(file)) != EOF)
    {
        if (line == n && col == m)
        {
            fseek(file, -1L, SEEK_CUR);
            fputc(x, file);
            break;
        }
        if (ch == '\n')
        {
            line++;
            col = 1;
        }
        else
        {
            col++;
        }
    }

    fclose(file);
}

// 获取当前日期并以 yyyy-MM-dd 格式输出
void get_date(char date_str[])
{
    time_t current_time;
    struct tm *local_time;
    char format[] = "%Y-%m-%d";

    // 获取当前时间
    current_time = time(NULL);
    local_time = localtime(&current_time);

    // 将日期格式化为字符串
    strftime(date_str, 11, format, local_time);
}

// 获取指定路径的文件的第 n 行
void get_line(const char *the_line, char *path, int n)
{
    FILE *file = fopen(path, "r"); // open file for reading
    if (file == NULL)
    {
        printf("File not found\n");
    }
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        count++;
        if (count == n)
        {
            fclose(file);
            strcpy(the_line, line);
            return;
        }
    }
    fclose(file);
}

int book_compensation()
{
    int status_code = admin_login();
    if (status_code == 1)
        return 1; // 返回主界面

    for (;;)
    {

        char ISBN[20];
        printf("Enter the ISBN\n");
        do
        {
            gets(ISBN);
        } while (!ISBN[0]);

        // 检测书本是否存在
        char path[40];
        strcpy(path, "./data/book/");
        strcat(path, ISBN);
        FILE *fp = fopen(path, "r+");
        if (fp != NULL)
            status_code = 1;
        else
        {
            fclose(fp);
            printf("Book does not exist\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
            status_code *= 2;
            // 0，重试
            // 1，下一步
            // 2，返回主界面
        }
        if (status_code == 2)
            return 1; // 返回主界面

        int book_number;

        printf("Enter book number\n");
        scanf("%d", &book_number);

        // 检测书本是否已被借出

        check_char(status_code, path, book_number, '1');
        if (status_code == -1)
        {
            printf("The book does not exist\n");
            continue;
        }
        else if (status_code == 0)
        {
            printf("The book has not been lent\n");
            continue;
        }

        // 输出价格
        char price[10];
        get_line(price, path, 4);

        int len = strlen(price); // 获取字符串长度
        if (len > 0)
        {
            price[len - 1] = '\0'; // 将字符串最后一个字符替换为字符串结束符
        }
        printf("The price of the book is:%.2f\n", atoi(price) * 0.01);

        // 遍历 ./data/student/ 文件夹中的文件，删除这条借书记录。
        char ISBNAND[40];
        strcpy(ISBNAND, ISBN);
        strcat(ISBNAND, "&");
        char book_numberc[10];
        itoa(book_number, book_numberc, 10);
        strcat(ISBNAND, book_numberc);
        strcat(ISBNAND, "&");
        remove_lines_start_with_string("./data/student/", ISBNAND);

        fclose(fp);
        printf("0,Continue returning books\n1,Back to Home page\n");
        scanf("%d", &status_code);
        if (status_code == 0)
            return 0; // 继续赔书
        return 1;     // 返回主界面
    }
}

book_search()
{
    int status_code;
    struct book books[MAX_BOOKS];
    int num_books;
    read_books("./data/book", books, &num_books);
    printf("%d books were read\n", num_books);

    // 搜索
    char key[20];
    puts("Enter keyword,enter \"0\" to not filter");
    do
    {
        gets(key);
    } while (!key[0]);

    if (strcmp(key, "0") != 0)
    {
        // printf("ok");
        remove_books(&books, &num_books, key);
    }

    // 排序
    puts("Do you want to sort the results?");
    puts("0,Yes\n1,No");
    scanf("%d", &status_code);
    if (!status_code)
    {

        int a, b;
        puts("Select sort key");
        puts("0,ISBN\n1,Title\n2,Author\n3,Publisher\n4,Price\n5,Status");
        scanf("%d", &a);
        puts("0,Ascending\n1,Descending");
        scanf("%d", &b);
        if (a == 0)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, ISBN, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, ISBN, DESCENDING);
            }
        }
        else if (a == 1)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, TITLE, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, TITLE, DESCENDING);
            }
        }
        else if (a == 2)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, AUTHOR, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, AUTHOR, DESCENDING);
            }
        }
        else if (a == 3)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, PUBLISHER, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, PUBLISHER, DESCENDING);
            }
        }
        else if (a == 4)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, PRICE, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, PRICE, DESCENDING);
            }
        }
        else if (a == 1)
        {
            if (b == 0)
            {
                sortBooks(books, num_books, STATUS, ASCENDING);
            }
            else if (b == 1)
            {
                sortBooks(books, num_books, STATUS, DESCENDING);
            }
        }
    }
    print_list(books, num_books);
    scanf("%d", &status_code);
    if (status_code == 0)
        return 0; // 继续搜
    return 1;     // 返回主界面
}

// 读取指定路径下的所有文件到 books 数组中
// 文件名以字符串形式读入到成员 ISBN，
// 第1行以字符串形式读入到成员 title，
// 第2行以字符串形式读入到成员 author，
// 第3行以字符串形式读入到成员 publisher，
// 第4行以整数形式读入到成员 price，
// 第5行以整数形式读入到成员 status
void read_books(const char *path, struct book *books, int *num_books)
{
    char filename[256];
    char line[256];
    FILE *file;
    int i = 0;

    *num_books = 0;

    // 构造文件路径
    sprintf(filename, "%s\\*", path);

    // 遍历文件夹
    WIN32_FIND_DATA find_data;
    HANDLE find_handle = FindFirstFile(filename, &find_data);
    if (find_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            // 跳过"."和".."文件夹
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
            {
                continue;
            }

            // 构造文件路径
            sprintf(filename, "%s\\%s", path, find_data.cFileName);

            // 打开文件
            file = fopen(filename, "r");
            if (file == NULL)
            {
                printf("Unable to open file %s\n", filename);
                continue;
            }

            // 读取文件内容
            strcpy(books[i].ISBN, find_data.cFileName);
            if (fgets(line, 256, file) != NULL)
            {
                strcpy(books[i].title, line);
                remove_last_char(books[i].title);
            }
            if (fgets(line, 256, file) != NULL)
            {
                strcpy(books[i].author, line);
                remove_last_char(books[i].author);
            }
            if (fgets(line, 256, file) != NULL)
            {
                strcpy(books[i].publisher, line);
                remove_last_char(books[i].publisher);
            }
            if (fgets(line, 256, file) != NULL)
            {
                books[i].price = atoi(line);
            }
            if (fgets(line, 256, file) != NULL)
            {
                strcpy(books[i].status, line);
            }

            // 关闭文件
            fclose(file);

            // 继续读取下一个文件
            i++;
        } while (FindNextFile(find_handle, &find_data) && i < MAX_BOOKS);
        FindClose(find_handle);
    }

    *num_books = i;
}

void swapBooks(struct book *book1, struct book *book2)
{
    struct book temp = *book1;
    *book1 = *book2;
    *book2 = temp;
}

// 可以对 struct book 类型的数组按照多种方式进行比较。
// 比较方式包括按 ISBN、标题、作者、出版商、价格、状态等字段进行比较。
int compareBooks(struct book book1, struct book book2, enum SortField sortField, enum SortOrder sortOrder)
{
    int result;
    switch (sortField)
    {
    case ISBN:
        result = strcmp(book1.ISBN, book2.ISBN);
        break;
    case TITLE:
        result = strcmp(book1.title, book2.title);
        break;
    case AUTHOR:
        result = strcmp(book1.author, book2.author);
        break;
    case PUBLISHER:
        result = strcmp(book1.publisher, book2.publisher);
        break;
    case PRICE:
        result = book1.price - book2.price;
        break;
    case STATUS:
        result = strcmp(book1.status, book2.status);
        break;
    default:
        result = 0;
        break;
    }
    if (sortOrder == DESCENDING)
    {
        result = -result;
    }
    return result;
}

// 可以对 struct book 类型的数组按照多种方式进行排序。
// 排序方式包括按 ISBN、标题、作者、出版商、价格、状态等字段进行升序或降序排序。
void sortBooks(struct book *books, int count, enum SortField sortField, enum SortOrder sortOrder)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (compareBooks(books[i], books[j], sortField, sortOrder) > 0)
            {
                swapBooks(&books[i], &books[j]);
            }
        }
    }
}

// 删除指定 struct book 类型数组中字符串类型成员均不包含指定字符串的变量，并将其它变量下标依次提前
// 即搜索功能
void remove_books(struct book *books, int *num_books, const char *keyword)
{
    int i, j;
    for (i = 0; i < *num_books; i++)
    {
        if (strstr(books[i].ISBN, keyword) == NULL &&
            strstr(books[i].title, keyword) == NULL &&
            strstr(books[i].author, keyword) == NULL &&
            strstr(books[i].publisher, keyword) == NULL)
        {
            // 如果 books[i] 不包含指定字符串，则将其删除
            for (j = i; j < *num_books - 1; j++)
            {
                // 将后面的变量依次提前一个位置
                books[j] = books[j + 1];
            }
            (*num_books)--;
            i--; // 这里要将 i 值减1，因为删除一个元素后，后面的元素会前移，需要重新检查 i 位置的元素。
        }
    }
}

// 删除字符串的最后一个字符
void remove_last_char(char *str)
{
    int len = strlen(str);
    if (len > 0)
    {
        str[len - 1] = '\0';
    }
}

//
void print_list(const struct book *books, int num_books)
{
    //         20                   20                  20                     20                   10      10
    printf("ISBN                Title               Author              Publisher           Price     In total  In collection\n");
    // printf("978-3-16-148410-0      A Book           A man");
    // num_books--;
    for (int i = 0; i < num_books; i++)
    {
        printf("%-20s", books[i].ISBN);
        printf("%-20s", books[i].title);
        printf("%-20s", books[i].author);
        printf("%-20s", books[i].publisher);
        printf("%-10.2f", books[i].price * 0.01);
        int n1, n0;
        count_char(books[i].status, '1', &n1);
        count_char(books[i].status, '0', &n0);
        printf("%-10d", n1 + n0);
        printf("%d\n", n1);
    }
    printf("0,Continue searching books\n1,Back to Home page\n");
}

// 统计指定字符串中指定字符出现的次数

void count_char(char *str, char ch, int *n)
{
    int count = 0;
    while (*str)
    {
        if (*str == ch)
            count++;
        str++;
    }
    *n = count;
}

int query_record()
{
    char name[20];
    char path[30];
    int status_code = 0;
    for (; status_code == 0;)
    {
        printf("Enter your student ID\n");
        do
        {
            gets(name);
        } while (!name[0]);
        strcpy(path, "./data/student/");
        strcat(path, name);
        FILE *fp = fopen(path, "r");
        if (fp != NULL)
        {
            status_code = 1;
            fclose(fp);
        }
        else
        {
            fclose(fp);
            printf("Username does not exist\n0,Retry\n1,Back to Home page\n");
            scanf("%d", &status_code);
            status_code *= 2;
            // 0，重试
            // 1，下一步
            // 2，返回主界面
        }
    }
    if (status_code == 2)
        return 1; // 返回主界面

    //         20                   20                  20                     20                   10      10
    printf("ISBN                Title               Author              Publisher           Price     Number  Borrowing time\n");
    char a[100];
    char b[100];
    char path_book[100];
    struct book books;

    FILE *fp = fopen(path, "r");
    int i = 1;
    do
    {
        get_line(&a, path, i + 3);
        remove_last_char(&a);
        extract_string(a, &b, 1);

        printf("%-20s", b);
        strcpy(path_book, "./data/book/");
        strcat(path_book, b);
        read_books_from_file(path_book, &books);
        printf("%-20s", books.title);
        printf("%-20s", books.author);
        printf("%-20s", books.publisher);
        printf("%-10.2f", books.price * 0.01);
        extract_string(a, &b, 2);
        printf("%-10s", b);
        extract_string(a, &b, 3);
        printf("%s\n", b);
        i++;

    } while (!a[0]);
    fclose(fp);
    return 1; // 返回主界面
}

// 提取指定字符串中第 n 个 x 字符到第 n-1 个 x 字符（n=1 则到开头）之间的字符串
void extract_string(char str[], char subStr[], int n)
{
    int len = strlen(str);
    int count = 0;
    int start = 0, end = len - 1;

    // 找到第n个x字符的位置
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '&')
        {
            count++;
            if (count == n)
            {
                end = i - 1;
                break;
            }
            start = i + 1;
        }
    }

    // 当n=1时，只提取到第一个x字符位置
    if (n == 1)
    {
        end = start + 2;
        start = 0;
    }

    // 复制子字符串
    int j = 0;
    for (int i = start; i <= end; i++)
    {
        subStr[j++] = str[i];
    }
    subStr[j] = '\0';
}

void read_books_from_file(const char *filename, struct book **books)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Error: failed to open file %s\n", filename);
        return;
    }

    char buffer[256];
    while (fgets(buffer, 256, fp))
    {
        // Allocate memory for the new book
        struct book *new_book = (struct book *)malloc(sizeof(struct book));
        if (!new_book)
        {
            printf("Error: failed to allocate memory for new book\n");
            return;
        }

        // 从文件的第一行读取标题
        if (!fgets(buffer, 256, fp))
        {
            printf("Error: unexpected end of file while reading title\n");
            free(new_book);
            return;
        }
        strncpy(new_book->title, buffer, 100);
        new_book->title[99] = '\0';

        // 从文件的第二行读取标题
        if (!fgets(buffer, 256, fp))
        {
            printf("Error: unexpected end of file while reading author\n");
            free(new_book);
            return;
        }
        strncpy(new_book->author, buffer, 100);
        new_book->author[99] = '\0';

        // 从文件的第三行读取出版社
        if (!fgets(buffer, 256, fp))
        {
            printf("Error: unexpected end of file while reading publisher\n");
            free(new_book);
            return;
        }
        strncpy(new_book->publisher, buffer, 100);
        new_book->publisher[99] = '\0';

        // 从文件的第四行读取价格
        if (!fgets(buffer, 256, fp))
        {
            printf("Error: unexpected end of file while reading price\n");
            free(new_book);
            return;
        }
        new_book->price = atoi(buffer);

        // 从文件的第五行读取借阅状态
        if (!fgets(buffer, 256, fp))
        {
            printf("Error: unexpected end of file while reading status\n");
            free(new_book);
            return;
        }
        strncpy(new_book->status, buffer, 100);
        new_book->status[99] = '\0';

        // 将新书添加到书籍数组中
        *books = new_book;
    }

    fclose(fp);
}