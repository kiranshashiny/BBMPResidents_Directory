#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

struct Person {

	string name;
	string parent;
	string house;
	int age;
	char gender;
};

struct Person residents [600];
int resCount=0;

string removeNonAscii(string line) {

    string result;

    for (unsigned char ch : line) {

        if (ch <= 127) {
            result += ch;
        }
    }

    return result;
}

// full line here.
vector<string> splitAgeGender(string line) {

	vector<string> parts;
	string item;

	stringstream ss(line);

	while (getline(ss, item, '|')) {
		  
	      // Remove leading spaces
	      while (!item.empty() && item[0] == ' ') {
	          item.erase(0, 1);
	      }
  
	      // Remove trailing spaces
	      while (!item.empty() && item.back() == ' ') {
	          item.pop_back();
	      }
  
	      parts.push_back(item);
	}
  
	for (int i = 0; i < parts.size(); i++) {
	 
		cout << "Part " << i + 1 << " = "
		           << parts[i] << endl;
		// do the age and the gender here.
		regex pattern(R"(Age\s*:\s*(\d+)\s*Gender\s*:\s*([MF]))");
	
		smatch match;
	
		if (regex_search(parts[i], match, pattern)) {
	
		    string age = match[1];
		    string gender = match[2];
	
		    cout << "Age    = " << age << endl;
		    cout << "Gender = " << gender << endl;
		}
		
	}

return parts;
}

vector<string> splitByPipe(string line) {

	vector<string> parts;

	stringstream ss(line);

	string item;

	while (getline(ss, item, '|')) {

	    // Remove leading spaces
	    while (!item.empty() && item[0] == ' ') {
	        item.erase(0, 1);
	    }

	    // Remove trailing spaces
	    while (!item.empty() && item.back() == ' ') {
	        item.pop_back();
	    }

	    parts.push_back(item);
	}

	return parts;
}

bool isEmptyLine(string line) {

	// Remove spaces/tabs/newlines
	for (char ch : line) {

	    if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') {
	        return false;
	    }
	}

	return true;
}

bool hasExactName(string line) {

	// Match only if line STARTS with "Name : "
	return line.rfind("Name", 0) == 0;
}

string processAgeGender(string line) {

	// Normalize spaces first
	line = regex_replace(line, regex("_"), "");

	// Remove first extra |
	if (line.rfind("| ", 0) == 0) {
	    line = line.substr(2);
	}

	// Normalize Gender formatting
	line = regex_replace(line, regex("Gender\\s*:\\s*"), "Gender : ");

	// Normalize Age formatting
	line = regex_replace(line, regex("Age\\s*:\\s*"), "Age : ");

	// full line here.
	//splitAgeGender ( line );

	return line;
}

bool startsWithNumber(string line) {

	return isdigit(line[0]);
}

bool isSerialNumberLine(string line) {

	// Matches:
	// 4 SOH5663174 5 SOH5662762 6 $OH4259552

	regex pattern(R"(^\d+\s+[A-Z\$]OH\d+)");

	return regex_search(line, pattern);
}

string processHouse(string line) {

	// Remove last pipe if present
	while (!line.empty() &&
	       (line.back() == ' ' || line.back() == '|')) {

	    if (line.back() == '|') {
	        line.pop_back();
	        break;
	    }

	    line.pop_back();
	}

	/* complete it later...
    vector<string> result = splitHouseByPipe(line);
	
	for (int i = 0; i < result.size(); i++) {
	
		cout << "Part " << i + 1 << " = "
	             << result[i] << endl;
	}
	*/
	
	return line;
}

void addResidents ( string firstline, string secondline ) {

	vector<string> firstresult = splitByPipe(firstline);
	vector<string> secondresult = splitByPipe(secondline);

	for (int i = 0; i < firstresult.size(); i++) {
	
		//cout << "NameShashi " << i + 1 << " = "
	    //        << firstresult[i] << "    " << secondresult[i] << endl;

	    residents[resCount].name = firstresult[i];
	    residents[resCount].parent = secondresult[i];
		resCount++;
	}

	return;
}


void printResidents () {

	cout << " ==================" << endl;

	for ( int i=0; i< resCount ; i++ ) {
		cout << residents[i].name << "  " <<  residents[i].parent << endl;
		
		cout << endl;
	}
}

string searchReplace(string haystack,
                     string needle,
                     string replacement) {

    size_t pos = 0;

    while ((pos = haystack.find(needle, pos))
            != string::npos) {

        haystack.replace(pos,
                         needle.length(),
                         replacement);

        pos += replacement.length();
    }

    return haystack;
}


string cleanupline ( string line ) {

    line = regex_replace(line, regex("Photo"), "");
    line = regex_replace(line, regex(";"), "");
    line = regex_replace(line, regex("!"), "");
    line = regex_replace(line, regex("|"), "");
    line = regex_replace(line, regex(";"), ":");
    line = regex_replace(line, regex("!"), "");
    line = regex_replace(line, regex("|"), "");
    line = regex_replace(line, regex("]"), "");
    line = regex_replace(line, regex("}"), "");
    line = regex_replace(line, regex("-"), "");
    line = regex_replace(line, regex("--"), "");
    line = regex_replace(line, regex("-"), "");

    replace(line.begin(), line.end(), '|', ' ');
    //line = regex_replace(line, regex("Available"), "");
    line = searchReplace(line, "Available", "");
    line = searchReplace(line, "|", "");
    line = searchReplace(line, "||", "");
	/*if (line.find("Available") != string::npos) {
 
		int pos = line.find("Available");
 
		line.replace(pos, 9, "|");
	} */

	line = removeNonAscii ( line );


	return line;
}

string addPipeSeparator ( string line ) {

	/*line = regex_replace(line,
	                regex("Father's Name"),
	                "| Father's Name");
    */
	line = regex_replace(
          line,
          regex(R"(Father[';]?s?\s+Name)"),
          "| Father's Name");

	line = regex_replace(line,
	               regex("Husband's Name"),
	                "| Husband's Name");

	line = regex_replace(line,
	                regex("Mother's Name"),
	                "| Mother's Name");

	line = regex_replace(line,
	                regex("Other's"),
	                "| Other's");

	        // Remove first extra |
	if (line[0] == '|')
	     line = line.substr(2);

	return line;
}

void splitNames (string line ) {

	vector<string> result = splitByPipe(line);
	for (int i = 0; i < result.size(); i++) {

		cout << "Part " << i + 1 << " = "
				 << result[i] << endl;
		//residents[resCount].name = result[i];
	}
}

void splitParentsNames (string line ) {

	vector<string> result = splitByPipe(line);
	for (int i = 0; i < result.size(); i++) {

		cout << "Parent " << i + 1 << " = "
            << result[i] << endl;
		//residents[resCount].parent = result[i];
	}
}
string addAgeGenderPipeSeparator ( string line ) {
	line = regex_replace(line,
       regex("Age"),
       "| Age");

       // Remove first extra |
	if (line[0] == '|')
		line = line.substr(2);
	return line;
}

int main(int argc, char *argv[]) {


	if ( argc < 2 ) {

 		cout << "Usage: ./program filename.txt" << endl;
	    return 1;
	}

	// Take filename from command line
	string filename = argv[1];

	ifstream file(filename);

	if (!file) {
	    cout << "Cannot open file" << filename << endl;
	    return 1;
	}

	string line;
	string firstline;
	string secondline;
	string thirdline;
	string fourthline;

	while (getline(file, line)) {

	    // Remove "Photo"

	   if ( isEmptyLine (line ) ){
		continue;
	   }
	   if ( isdigit(line[0] )) {
		  cout << endl;
		  continue; // Skip line
	   } 
		line = cleanupline ( line ) ;

	    // Names line  // 1st line

	    if (hasExactName(line)) {

	        line = regex_replace(line,
	                regex("Name"),
	                "| Name");

	        // Remove first extra |
	        if (line[0] == '|')
	            line = line.substr(2);
			firstline = line;
			//splitNames (firstline);

			cout << "1:" <<  firstline << endl;
			continue;
	    }

	    // Husband/Father line // 2nd line
	    if (line.find("Husband's Name") != string::npos ||
	        line.find("Mother's Name") != string::npos ||
	        line.find("Other's") != string::npos ||
	        line.find("Fathers Name") != string::npos ||
	        line.find("Father's Name") != string::npos) 
	    {
			//replace(line.begin(), line.end(), ' ', '.');

			line = addPipeSeparator ( line );

			secondline = line;
			//splitParentsNames ( secondline );
			cout << "2:" <<  secondline << endl;
		    addResidents( firstline, secondline );
		    
	    }

/*
	    // Process House address H.No lines // 3rd line.
	    if (line.find("H.INo") != string::npos ||
	        line.find("H,INo") != string::npos ||
	        line.find("H. No") != string::npos ||
	        line.find("H.. No") != string::npos ||
	        line.find("H..No") != string::npos ||
	        line.find("H, ") != string::npos ||
	        line.find("H, No") != string::npos ||
	        line.find("1. No") != string::npos) {
   	    	// 
			thirdline = line;
		    //cout << "3:" << thirdline << endl;
		    
		    //processHouse (thirdline ); // 3rd line.
	        continue;
	    }

	    // Age/Gender line // last line for this para.
	    if (line.find("Age") != string::npos &&
	        line.find("Gender") != string::npos) {

			fourthline = addAgeGenderPipeSeparator(line );
	    	//fourthline = processAgeGender(fourthline) ;
			cout <<  fourthline << endl;
	    }
*/
	}
	cout << "Total " << resCount << endl;

	file.close();
	cout <<  "================" << endl;
	printResidents ();

	return 0;
}
