#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std; 

/**********************************************************************
* Generates a query based on the parameters it receives
***********************************************************************/
string queryGenerator(string username, string password) {
    string statement = "INSERT INTO users VALUES ('" + username + "', '" + password + "');";
    return statement;
}

/**********************************************************************
* Passes 12 pairs of usernames and passwords to the generator function.
***********************************************************************/
void testCases() {
    queryGenerator("user1", "password1");
    queryGenerator("user2", "password2");
    queryGenerator("user3", "password3");
    queryGenerator("user4", "password4");
    queryGenerator("user5", "password5");
    queryGenerator("user6", "password6");
    queryGenerator("user7", "password7");
    queryGenerator("user8", "password8");
    queryGenerator("user9", "password9");
    queryGenerator("user10", "password10");
    queryGenerator("user11", "password11");
    queryGenerator("user12", "password12");
}

/**********************************************************************
* Gets test queries
***********************************************************************/
std::vector<std::string> getTestQueries()
{
  std::vector<std::string> data;
  data.push_back("hello");
    // Generate 12 test cases to represent valid input.
    // Have a function to call all the test cases.

  return data;
}

/**********************************************************************
* Authenticates a user in "SQL" and returns true or false
***********************************************************************/
string checkAuthentication(string username, string password)
{
  string returnVal = "SELECT * FROM user_details WHERE userid ='" + username + "' and password ='" + password + "';";
    //Query Generation: Accept two strings, return a single string to check if the user is authenticated. 
    return returnVal; //SELECT * FROM user_details WHERE userid ='' UNION SELECT * FROM EMP_DETAILS -- ' and password =  'abcd'
}

/**********************************************************************
* Get test strings for various functions
* Type 1 returns tautology attacks
* Type 2 returns Union Query attacks
* Type 3 returns Comment attacks
* Type 4 returns Additional statement attacks
***********************************************************************/
std::vector<std::string> getTestVulnerabilities(int attackType)
{
  std::vector<std::string> data;
  switch (attackType)
  {
    case 1: //tautology attacks, pattern goes, username, then pass, then username etc..
    {
       data.push_back("Joe"); //username
       data.push_back("anything' or '1' = '1'"); //pass
       
    }
    case 2: // union query
    {
       data.push_back("jam' UNION SELECT ‘a’,NULL,NULL,NULL --"); 
       data.push_back("hack' UNION SELECT username FROM accounts");
    }
    case 3:
    {
       data.push_back("jam';"); // comment attack
       data.push_back("jam\";"); // comment attack
       
    }
    case 4:
    {
       data.push_back("; rm -R ./"); //additional statement attack
       data.push_back(" && rm -R ./"); //additional statement attack
    }
  }
    // Vulnerabilities: Generate test cases for tautology, 
    // union query, comment and additional statement attacks.

  return data;
}

/**********************************************************************
* Runs weak mitigation on the value passed in and returns it.
***********************************************************************/
string weakMitigation(string Value)
{
    //Weak Mitigation against UNION Query Attack
    const string matchUnion = "UNION";
    std::size_t foundUnion = Value.find(matchUnion);
    if (foundUnion != std::string::npos) {
        //Delete substring after found
        Value.erase(Value.find(matchUnion), Value.length());
    }

    //Weak Mitigation against Tautology Attack
    const string matchQuote = "'";
    const string matchOR = "OR";
    std::size_t foundQuote = Value.find(matchQuote);
    std::size_t foundOR = Value.find(matchOR);
    if (foundQuote != std::string::npos) {
        //Delete substring after found
        Value.erase(Value.find(matchQuote), Value.length());
    } 
    if (foundOR != std::string::npos) {
        //Delete substring after found
        Value.erase(Value.find(matchOR), Value.length());
    }

    //Weak Mitigation against Comment Attack
    const string matchComment = "--";
    std::size_t foundComment = Value.find(matchComment);
    if (foundComment != std::string::npos) {
        //Delete substring after found
        Value.erase(Value.find(matchComment), Value.length());
    }

    //Weak Mitigation against Additional Statement Attack
    const string matchColon = ";";
    std::size_t foundColon = Value.find(matchColon);
    if (foundColon != std::string::npos) {
        //Delete substring after found
        Value.erase(Value.find(matchColon), Value.length());
    }
 
    return Value;
}

/**********************************************************************
* Ensures complete sanitization of the string then returns it.
***********************************************************************/
string strongMitigation(string Value)
{
    Value = weakMitigation(Value);
    vector<string> array;
    stringstream ss(Value);
    string tmp;
    string sqlArray [] = {
            "ADD","ALTER","ALL","AND","ANY","AS","ASC","BACKUP DATABASE","BETWEEN","CASE","CHECK",
            "COLUMN","CONSTRAINT","CREATE","DATABASE","DEFAULT","DELETE","DESC","DISTINCT","DROP","EXEC",
            "EXISTS","FOREIGN KEY","FROM","FULL OUTER JOIN","GROUP BY","HAVING","IN","INDEX","INNER JOIN",
            "INSERT INTO","IS NULL","IS NOT NULL","JOIN","LEFT JOIN","LIKE","LIMIT","NOT","NOT NULL","OR",
            "ORDER BY","OUTER JOIN","PRIMARY","PROCEDURE","RIGHT JOIN","ROWNUM","SELECT","SET","TABLE",
            "TOP","TRUNCATE TABLE","UNION","UNION ALL","UNIQUE","UPDATE","VALUES","VIEW","WHERE"
    };
    while(std::getline(ss, tmp, ' '))
    {
        array.push_back(tmp);
    }
    for (int i = 0; i < array.size(); i++) {
        for (int j = 0; j < 58; j++) { //58 is the size of sqlArray. sqlArray.size does not work for string arrays.
            if (array[i] == sqlArray[j]) {
                for (int k = i; k < array.size(); k++) {
                    array[k] = array[k + 1];
                }
            }
        }
    }
    string newVal = "";
    for (int i = 0; i < array.size(); i++) {
        newVal = newVal + array[i];
    }
    Value = newVal;
    // Strong Mitigation: create a function which calls 
    // weak mitigation and adds to it until it is 
    // considered strong mitigation.
    return Value;
}

string strongUnionMitigation(string Value){
    string result = "";
    result = weakMitigation(Value) + "';";
    
    return result;
}

/**********************************************************************
* Runs a series of tests for security sanitization and mitigation.
***********************************************************************/
int main()
{
    string apple = " SELECT OR DESIGN OR APPLE OROR ERROR ; ASDF / -- ' awer '";
    cout << weakMitigation(apple);
    vector<string> tautologyTests = getTestVulnerabilities(1);
    vector<string> unionQueryTests = getTestVulnerabilities(2);
    vector<string> commentTests = getTestVulnerabilities(3);
    vector<string> additionalStatementTests = getTestVulnerabilities(4);
    cout << endl << "------------------------------------";
    cout << endl << "-------Query Generation Tests-------";
    cout << endl << "------------------------------------";
    testCases();
    cout << endl << checkAuthentication("Jordan","1234");
    cout << endl << checkAuthentication("Parker","IMaBeas7");
    cout << endl << checkAuthentication("Mary","DidYouKnow?");
    
   //Stuff goes here
   return 0;
}
