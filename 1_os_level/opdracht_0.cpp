#include <iostream>
#include <string>

using namespace std;

// Ask for string to convert
string ask_string(){
    string response_message;
    cout << "Enter string you whould like to decode" << endl;
    getline(cin, response_message);

    cout << endl;
    return response_message;
}

// Ask for shifting direction
string ask_direction(){
    
    string response_direction;
    cout << "Enter a direction [left/right]" << endl;
    getline(cin, response_direction);

    cout << endl;
    return response_direction;
}

// Ask for amount
int ask_number(){
    
    int response_number;
    cout << "How many character do you want to shift in that direction? Range: 1 - 4" << endl;
    cin >> response_number;

    bool check = true;

    while(check == true){
        if(response_number > 0 || response_number <= 4){
            check = false;
        } else {
            cout << endl;
            cout << "Use a number in the range of 1 - 4" << endl;
        }
    }
    
    cout << endl;
    return response_number;
}

int main(){
    
    string message = ask_string();
    string direction = ask_direction();
    int number = ask_number();

    string result;
    char new_letter;
   
    for(unsigned int i=0; i < message.size(); i++){

        if(message[i] != ' '){

            if(direction == "Left" || direction == "left"){
                new_letter = message[i] - number;
                result += new_letter;
                

            } else {
                new_letter = message[i] + number;
                result += new_letter;
            }

        } else {
            result += message[i];
        }
    }

    cout << "New string converted: " << endl;
    cout << result << endl;
    cout << endl;

}