#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "stemmer.h" // Include the Porter Stemming Algorithm header
using namespace std;

// Function to read a book into a string
string readBook(string path) {
    string text;
    fstream newfile;
    newfile.open(path, ios::in);
    if (newfile.is_open()) {
        string line;
        while (getline(newfile, line)) {
            text += line + " ";
        }
        newfile.close();
    }
    return text;
}

// Function to read emotion words from a file into a vector
void readEmotionFile(string path, vector<string> &emotionWords) {
    fstream newfile;
    newfile.open(path, ios::in);
    if (newfile.is_open()) {
        string line;
        while (getline(newfile, line)) {
            emotionWords.push_back(line);
        }
        newfile.close();
    }
}

// Function to split the text into words based on delimiters
vector<string> splitIntoWords(const string &text, const string &delimiters) {
    vector<string> words;
    size_t start = 0, end = 0;

    while ((start = text.find_first_not_of(delimiters, end)) != string::npos) {
        end = text.find_first_of(delimiters, start);
        words.push_back(text.substr(start, end - start));
    }

    return words;
}

// Function to apply stemming using the Porter Algorithm
vector<string> stemWords(const vector<string> &words) {
    vector<string> stemmedWords;
    for (const string &word : words) {
        stemmedWords.push_back(stemString(word)); 
    }
    return stemmedWords;
}

// Function to count words matching emotion categories
int countEmotionWords(const vector<string> &words, const vector<string> &emotionWords) {
    int count = 0;
    for (const string &word : words) {
        for (const string &emotionWord : emotionWords) {
            if (word == emotionWord) {
                count++;
            }
        }
    }
    return count;
}
// Function to extract context around a character
string extractContext(const string &text, const string &characterName, size_t range = 100) {
    size_t pos = text.find(characterName);
    if (pos != string::npos) {
        size_t start = (pos >= range) ? pos - range : 0;
        size_t end = min(pos + range, text.length());
        return text.substr(start, end - start);
    }
    return ""; // Return empty string if character not found
}
int countOccurrences(const string &text, const string &characterName) {
    size_t pos = 0;
    int count = 0;

    while ((pos = text.find(characterName, pos)) != string::npos) {
        count++;
        pos += characterName.length(); // Move past this occurrence
    }

    return count;
}
// Function to count emotion words around a character's name
int countEmotionWordsAroundCharacter(
    const string &text, 
    const string &characterName, 
    const vector<string> &emotionWords, 
    size_t range = 100) 
{
    string delimiters = ".,!~/(){}\\-_$@#%^&*;:'\" \n\t";
    // Tokenize the text
    vector<string> words = splitIntoWords(text, delimiters);
    vector<string> stemmedWords = stemWords(words);

    int emotionWordCount = 0;
    size_t totalWords = words.size();

    for (size_t i = 0; i < totalWords; ++i) {
        if (words[i] == characterName) {
            // Define the range
            size_t start = (i >= range) ? i - range : 0;
            size_t end = min(i + range, totalWords - 1);

            // Count emotion words in the range
            for (size_t j = start; j <= end; ++j) {
                for (const string &emotionWord : emotionWords) {
                    if (stemmedWords[j] == emotionWord) {
                        emotionWordCount++;
                    }
                }
            }
        }
    }

    return emotionWordCount;
}



int main() {
    // Define delimiters
    string delimiters = ".,!~/(){}\\-_$@#%^&*;:'\" \n\t";

    // Read the book
    string book1 = readBook("pride_and_prejudice.txt");
    string book2 = readBook("a_tale_of_two_cities.txt");
    // Characters
    string character1 = "Darcy";
    string character2 = "Wickham";
    int count1 = countOccurrences(book1, character1);
    int count2 = countOccurrences(book1, character2);
    // Split book1 into words
    vector<string> book1Words = splitIntoWords(book1, delimiters);
    vector<string> book2Words = splitIntoWords(book2, delimiters);

    // Apply stemming to the words
    vector<string> stemmedWords = stemWords(book1Words); //book1
    vector<string> stemmed2Words = stemWords(book2Words); //book2


    // Read emotion words 
    vector<string> angerWords;
    readEmotionFile("./emotion_word_data/anger.txt", angerWords);
    vector<string> confusionWords;
    readEmotionFile("./emotion_word_data/confusion_helplessness.txt", confusionWords);
    vector<string> HappyAliveWords;
    readEmotionFile("./emotion_word_data/happy_alive.txt", HappyAliveWords);
    vector<string> inspiredWords;
    readEmotionFile("./emotion_word_data/inspired.txt", inspiredWords);
    vector<string> relaxedWords;
    readEmotionFile("./emotion_word_data/relaxed_peaceful.txt", relaxedWords);
    vector<string> safeWords;
    readEmotionFile("./emotion_word_data/safe_satisfied.txt", safeWords);
    vector<string> urgencyWords;
    readEmotionFile("./emotion_word_data/urgency.txt", urgencyWords);


    // Stem the emotion words as well
    vector<string> stemmedAngerWords = stemWords(angerWords);
    vector<string> stemmedConfusionWords = stemWords(confusionWords);
    vector<string> stemmedHappyWords = stemWords(HappyAliveWords);
    vector<string> stemmedinspiredWords = stemWords(inspiredWords);
    vector<string> stemmedrelaxedWords = stemWords(relaxedWords);
    vector<string> stemmedsafeWords = stemWords(safeWords);
    vector<string> stemmedurgencyWords = stemWords(urgencyWords);

    // Count Emotion words in book1
    int angerCount = countEmotionWords(stemmedWords, stemmedAngerWords);
    int confusionCount = countEmotionWords(stemmedWords, stemmedConfusionWords);
    int happyCount = countEmotionWords(stemmedWords, stemmedHappyWords);
    int inspiredCount = countEmotionWords(stemmedWords, stemmedinspiredWords);
    int relaxedCount = countEmotionWords(stemmedWords, stemmedrelaxedWords);
    int safeCount = countEmotionWords(stemmedWords, stemmedsafeWords);
    int urgencyCount = countEmotionWords(stemmedWords, stemmedurgencyWords);
    // Count Emotion words in book2
    int angerCount2 = countEmotionWords(stemmed2Words, stemmedAngerWords);
    int confusionCount2 = countEmotionWords(stemmed2Words, stemmedConfusionWords);
    int happyCount2 = countEmotionWords(stemmed2Words, stemmedHappyWords);
    int inspiredCount2 = countEmotionWords(stemmed2Words, stemmedinspiredWords);
    int relaxedCount2 = countEmotionWords(stemmed2Words, stemmedrelaxedWords);
    int safeCount2 = countEmotionWords(stemmed2Words, stemmedsafeWords);
    int urgencyCount2 = countEmotionWords(stemmed2Words, stemmedurgencyWords);

    // Display the results
   
  

    // Calculate the percentage of book1
double book1angerPercentage = (static_cast<double>(angerCount) / book1Words.size()) * 100;
double book1happyPercentage = (static_cast<double>(happyCount) / book1Words.size()) * 100;
double book1confusionPercentage = (static_cast<double>(confusionCount) / book1Words.size()) * 100;
double book1relaxPercentage = (static_cast<double>(inspiredCount) / book1Words.size()) * 100;
double book1inspiredPercentage = (static_cast<double>(relaxedCount) / book1Words.size()) * 100;
double book1safePercentage = (static_cast<double>(safeCount) / book1Words.size()) * 100;
double book1urgencyPercentage = (static_cast<double>(urgencyCount) / book1Words.size()) * 100;
    //Calculate the percentage of book2
double book2angerPercentage = (static_cast<double>(angerCount2) / book2Words.size()) * 100;
double book2happyPercentage = (static_cast<double>(happyCount2) / book2Words.size()) * 100;
double book2confusionPercentage = (static_cast<double>(confusionCount2) / book2Words.size()) * 100;
double book2relaxPercentage = (static_cast<double>(relaxedCount2) / book2Words.size()) * 100;
double book2inspiredPercentage = (static_cast<double>(inspiredCount2) / book2Words.size()) * 100;
double book2safePercentage = (static_cast<double>(safeCount2) / book2Words.size()) * 100;
double book2urgencyPercentage = (static_cast<double>(urgencyCount2) / book2Words.size()) * 100;

// Count emotion words around specific characters
int darcyHappyCount = countEmotionWordsAroundCharacter(book1, character1, stemmedHappyWords, 100);
int darcyAngryCount = countEmotionWordsAroundCharacter(book1, character1, stemmedAngerWords, 100);
int darcyConfusionCount = countEmotionWordsAroundCharacter(book1, character1, stemmedConfusionWords, 100);
int darcyinspiredCount = countEmotionWordsAroundCharacter(book1, character1, stemmedinspiredWords, 100);
int darcyrelaxedCount = countEmotionWordsAroundCharacter(book1, character1, stemmedrelaxedWords, 100);
int darcysafeCount = countEmotionWordsAroundCharacter(book1, character1, stemmedsafeWords, 100);
int darcyurgencyCount = countEmotionWordsAroundCharacter(book1, character1, stemmedurgencyWords, 100);
// WIckham
int wickhamHappyCount = countEmotionWordsAroundCharacter(book1, character2, stemmedHappyWords, 100);
int wickhamAngryCount = countEmotionWordsAroundCharacter(book1, character2, stemmedAngerWords, 100);
int wickhamConfusionCount = countEmotionWordsAroundCharacter(book1, character2, stemmedConfusionWords, 100);
int wickhaminspiredCount = countEmotionWordsAroundCharacter(book1, character2, stemmedinspiredWords, 100);
int wickhamrelaxedCount = countEmotionWordsAroundCharacter(book1, character2, stemmedrelaxedWords, 100);
int wickhamsafeCount = countEmotionWordsAroundCharacter(book1, character2, stemmedsafeWords, 100);
int wickhamurgencyCount = countEmotionWordsAroundCharacter(book1, character2, stemmedurgencyWords, 100);

// Calculate Percentage for Character1
double DarcyAngryPercentage = (static_cast<double>(darcyAngryCount) / (count1 * 200)) * 100;
double DarcyHappyPercentage = (static_cast<double>(darcyHappyCount) / (count1 * 200)) * 100;
double DarcyrelaxedPercentage = (static_cast<double>(darcyrelaxedCount) / (count1 * 200)) * 100;
double DarcyConfusedHappyPercentage = (static_cast<double>(darcyConfusionCount) / (count1 * 200)) * 100;
double DarcyInspiredPercentage = (static_cast<double>(darcyinspiredCount) / (count1 * 200)) * 100;
double DarcySafePercentage = (static_cast<double>(darcysafeCount) / (count1 * 200)) * 100;
double DarcyUrgencyPercentage = (static_cast<double>(darcyurgencyCount) / (count1 * 200)) * 100;
// Calculate Percentafe for Character2
double WickHamAngryPercentage = (static_cast<double>(wickhamAngryCount) / (count1 * 200)) * 100;
double WickhamHappyPercentage = (static_cast<double>(wickhamHappyCount) / (count1 * 200)) * 100;
double WickhamrelaxedPercentage = (static_cast<double>(wickhamrelaxedCount) / (count1 * 200)) * 100;
double WickhamConfusedPercentage = (static_cast<double>(wickhamConfusionCount) / (count1 * 200)) * 100;
double WickhamInspiredPercentage = (static_cast<double>(wickhaminspiredCount) / (count1 * 200)) * 100;
double WickhamSafePercentage = (static_cast<double>(wickhamsafeCount) / (count1 * 200)) * 100;
double WickhamUrgencyPercentage = (static_cast<double>(wickhamurgencyCount) / (count1 * 200)) * 100;

// Display results

cout << "Happywords around " << character1 << ": " << darcyHappyCount << endl;
cout << "Anger around " << DarcyAngryPercentage << ": " << darcyAngryCount << endl;
cout << "Happy around " << DarcyHappyPercentage << ": " << darcyHappyCount << endl;

// Display the percentage
 cout << left << setw(25) << "Emotion"
         << setw(25) << "Pride and Prejudice (%)"
         << setw(25) << "A Tale of Two Cities (%)"
         << "\n";
     cout << left << setw(25) << "Anger"
         << setw(25) << setprecision(2) <<  book1angerPercentage
         << setw(25) << setprecision(2) << book2angerPercentage << "\n";

    cout << left << setw(25) << "Confusion"
         << setw(25) << book1confusionPercentage
         << setw(25) << book2confusionPercentage << "\n";
    cout << left << setw(25) << "Happy"
         << setw(25) << book1happyPercentage
         << setw(25) << book2happyPercentage << "\n";
     cout << left << setw(25) << "Inspired"
         << setw(25) << book1inspiredPercentage
         << setw(25) << book2inspiredPercentage << "\n";
    cout << left << setw(25) << "Relax"
         << setw(25) << book1relaxPercentage
         << setw(25) << book2relaxPercentage << "\n";
    cout << left << setw(25) << "Safe"
         << setw(25) << book1safePercentage
         << setw(25) << book2safePercentage << "\n";

    cout << left << setw(25) << "Urgency"
         << setw(25) << book1urgencyPercentage
         << setw(25) << book2urgencyPercentage << "\n" << endl;
    

    // Display the Character percentage
 cout << left << setw(25) << "Emotion"
         << setw(25) << "Darcy (%)"
         << setw(25) << "Wickham (%)"
         << "\n";
     cout << left << setw(25) << "Anger"
         << setw(25) << setprecision(2) <<  DarcyAngryPercentage
         << setw(25) << setprecision(2) << WickHamAngryPercentage << "\n";

    cout << left << setw(25) << "Confusion"
         << setw(25) << DarcyConfusedHappyPercentage
         << setw(25) << WickhamConfusedPercentage << "\n";
    cout << left << setw(25) << "Happy"
         << setw(25) << DarcyHappyPercentage
         << setw(25) << WickhamHappyPercentage << "\n";
     cout << left << setw(25) << "Inspired"
         << setw(25) << DarcyInspiredPercentage
         << setw(25) << WickhamInspiredPercentage << "\n";
    cout << left << setw(25) << "Relax"
         << setw(25) << DarcyrelaxedPercentage
         << setw(25) << WickhamrelaxedPercentage << "\n";
    cout << left << setw(25) << "Safe"
         << setw(25) << DarcySafePercentage
         << setw(25) << WickhamSafePercentage << "\n";

    cout << left << setw(25) << "Urgency"
         << setw(25) << DarcyUrgencyPercentage
         << setw(25) << WickhamUrgencyPercentage << "\n";
   

    //Display result for charcters 
       cout << "The character '" << character1 << "' appears " << count1 << " times in the book." << endl;
    cout << "The character '" << character2 << "' appears " << count2 << " times in the book." << endl;



    return 0; 
}
