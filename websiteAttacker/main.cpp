/*

Bruteforce attacking MySQL database using PRTBruteforce.
- Akshay Mohan


This code is wrapped up for quick use, so it's very untidy!


Consider having this query done for illustration:
SET GLOBAL max_prepared_stmt_count = 999999;

Using prepared statements to avoid SQL injections through application.

*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "PRTBruteforce/src/bruteforce.h"

//MySQL connection configuration.
#define         DB_HOST         "localhost"
#define         DB_USER         "someuser"
#define         DB_PASS         "123456"
#define         DB_NAME         "userdb"


//Overloading |= operator to support BRUTEFORCE_FLAGS
inline BRUTEFORCE_FLAGS operator|= (BRUTEFORCE_FLAGS &a, BRUTEFORCE_FLAGS b) {

	return (BRUTEFORCE_FLAGS)((int &)a |= (int)b);
}



int main(int argc, char **argv) {

    BRUTEFORCE_HANDLE bf_handle;
    BRUTEFORCE_FLAGS bf_flags = (BRUTEFORCE_FLAGS)0;

    char
        sUsername[24],
        sStartPoint[16],
        *startPtr,
        cResponse;

    int pass_max_length;
    unsigned long long p_stmt_counts = 0;

    sql::Driver             *sql_driver;
    sql::Connection         *sql_conn;
    sql::PreparedStatement  *pstmt;
    sql::ResultSet          *sql_result;

    printf("Enter username : ");
    scanf("%[^\n]s", sUsername);

    printf("Is the password expected to have lowercase letters? (Y/N) : ");
    scanf(" %c", &cResponse);

    if(tolower(cResponse) == 'y')
        bf_flags |= BF_FLAG_LOWERCASE;

    printf("Is the password expected to have uppercase letters? (Y/N) : ");
    scanf(" %c", &cResponse);

    if(tolower(cResponse) == 'y')
        bf_flags |= BF_FLAG_UPPERCASE;

    printf("Is the password expected to have digits? (Y/N) : ");
    scanf(" %c", &cResponse);

    if(tolower(cResponse) == 'y')
        bf_flags |= BF_FLAG_DIGITS;

    printf("Is the password expected to have symbols? (Y/N) : ");
    scanf(" %c", &cResponse);

    if(tolower(cResponse) == 'y')
        bf_flags |= BF_FLAG_SYMBOLS;
    
    printf("Enter the expected maximum password length : ");
    scanf("%d", &pass_max_length);

    if(pass_max_length > 5 || pass_max_length < 1) {

        printf("To illustrate bruteforce quickly, passwords are considered to have 1-5 characters.\n");
        printf("Changing password max length to 5.\n");

        pass_max_length = 5;
    }
    printf("Enter a start point if any (Enter 'a' if there's no start point) : ");
    scanf(" %[^\n]s", sStartPoint);

    startPtr = (!strcmp(sStartPoint, " ")) ? (NULL) : (sStartPoint);
    //startPtr = sStartPoint;
    bruteforce_init(&bf_handle, pass_max_length, bf_flags, NULL, startPtr);
    /*while(bruteforce_update(&bf_handle)) {

        printf("%s\n", bf_handle.bfText);
    }
    bruteforce_finalize(&bf_handle);*/
    try {

        sql_driver  = get_driver_instance();
        sql_conn    = sql_driver->connect(DB_HOST, DB_USER, DB_PASS);
        sql_conn->setSchema(DB_NAME);

        while(bruteforce_update(&bf_handle)) {

            pstmt = sql_conn->prepareStatement("SELECT `userid` FROM `user` WHERE `username`=? AND `userpass`=?");

            pstmt->setString(1, sUsername);
            pstmt->setString(2, bf_handle.bfText);

            sql_result = pstmt->executeQuery();
            printf("Trying : %s\n", bf_handle.bfText);
            if(sql_result->next()) {

                printf("Password cracked!\nAccount name : %s\nPassword : %s\n",
                    sUsername, bf_handle.bfText);
                break;
            }
            delete sql_result;
            delete pstmt;
        }
        delete sql_result;
        delete pstmt;
        delete sql_conn;
        bruteforce_finalize(&bf_handle);
    } catch (sql::SQLException &e) {

        std::cout << "SQL Exception in " << __FILE__ << std::endl;
        std::cout << "ERROR : " << e.what() << std::endl;
        std::cout << "MySQL error code: " << e.getErrorCode() << std::endl;
        std::cout << "SQL State: " << e.getSQLState() << std::endl;
    }
    return 0;
}