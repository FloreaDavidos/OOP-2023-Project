#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

using namespace std;

// Funcție pentru a obține data și ora curentă
string getCurrentDateTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
    return buffer;
}

// Clasa abstractă pentru un pas în flow (abstract class Step)
class Step {
    public:
        virtual void getStepInfo() = 0;
        virtual string getStepName() = 0;
        virtual void writeToFile(ofstream &file) const = 0;
        virtual ~Step() {}
};

// Pasul de sfârșit (EndStep) - derivat din clasa abstractă Step
class EndStep : public Step {
public:
    string getStepName() override {
        return "End Step";
    }

    void getStepInfo() override {
        cout << "Sfarsitul flow-ului." << endl;
    }

    void writeToFile(ofstream &file) const {
        file << "EndStep" << endl;
    }
};

class TextFileInputStep : public Step {
private:
    string fileName;
    string description;

public:
    TextFileInputStep(const string &fileName) : fileName(fileName) {}

    string getStepName() override {
        return "Text File Input Step";
    }

    void specifyDescription() {
        cout << "Introduceti descrierea pentru pasul '" << fileName << "': ";
        cin.ignore();  // Ignorăm orice caracter rămas în buffer
        getline(cin, description);
    }

    void getStepInfo() override {
        cout << "Fisier: " << fileName << endl;
        cout << "Descriere: " << description << endl;
    }

    const string &getFileName() const {
        return fileName;
    }

    const string &getDescription() const {
        return description;
    }

    void createFile() const {
    ofstream file(fileName + ".txt");
    if (file.is_open()) {
        file << description << endl;
        file.close();
        cout << "Fisier creat cu succes!" << endl;
    } else {
        // Am adăugat un bloc try-catch pentru a gestiona excepția în caz de eroare la crearea fișierului
        try {
            throw runtime_error("Eroare la crearea fisierului!");
        } catch (const exception &e) {
            cerr << e.what() << endl;
        }
    }
}

    void writeToFile(ofstream &file) const {
        file << "TextFileInputStep" << endl;
        file << "Fisier: " << fileName << ".txt" << endl;
        file << "Descriere: " << description << endl;
    }
};

class CSVFileInputStep : public Step {
private:
    string fileName;
    string description;

public:
    CSVFileInputStep(const string &fileName) : fileName(fileName) {}

    string getStepName() override {
        return "CSV File Input Step";
    }

    void specifyDescription() {
        cout << "Introduceti descrierea pentru pasul '" << fileName << "': ";
        cin.ignore();  // Ignorăm orice caracter rămas în buffer
        getline(cin, description);
    }

    void getStepInfo() override {
        cout << "Fisier: " << fileName << endl;
        cout << "Descriere: " << description << endl;
    }

    const string &getFileName() const {
        return fileName;
    }

    const string &getDescription() const {
        return description;
    }

    void createFile() const {
        ofstream file(fileName + ".csv");
        if (file.is_open()) {
            file << "Descriere: " << description << endl;
            file.close();
            cout << "Fisier CSV creat cu succes!" << endl;
        } else {
            cout << "Eroare la crearea fisierului CSV!" << endl;
        }
    }

    void writeToFile(ofstream &file) const {
        file << "TextFileInputStep" << endl;
        file << "Fisier: " << fileName << ".csv" << endl;
        file << "Descriere: " << description << endl;
    }
};

class OutputStep : public Step {
private:
    string fileName;
    size_t stepNumber;
    string title;

public:
    OutputStep(const string &fileName, size_t stepNumber, const string &title)
        : fileName(fileName), stepNumber(stepNumber), title(title) {}

    string getStepName() override {
        return "Output Step";
    }

    void specifyInfo() {
        cout << "Introduceti numele fisierului pentru Output Step: ";
        cin >> fileName;

        cout << "Introduceti numarul step-ului din flow pentru care doriti informatiile: ";
        cin >> stepNumber;

        cout << "Introduceti titlul pentru prima linie din fisier: ";
        cin.ignore();
        getline(cin, title);
    }

    const string &getFileName() const {
        return fileName;
    }

    void getStepInfo() override {
        cout << "Fisier: " << fileName << endl;
        cout << "Numar step: " << stepNumber << endl;
        cout << "Titlu: " << title << endl;
    }

    void createFile(const vector<Step *> &steps) const {
        if (stepNumber >= 1 && stepNumber <= steps.size()) {
            ofstream file(fileName + ".txt");
            if (file.is_open()) {
                file << title << endl;

                // Obține informațiile pentru step-ul specificat
                steps[stepNumber - 1]->writeToFile(file);

                file.close();
                cout << "Fisier text creat cu succes!" << endl;
            } else {
                cout << "Eroare la crearea fisierului text!" << endl;
            }
        } else {
            cout << "Numarul step-ului specificat nu exista in flow!" << endl;
        }
    }

    void writeToFile(ofstream &file) const {
        file << "OutputStep" << endl;
        file << "Filename: " << fileName << ".txt" << endl;
        file << "StepNumber: " << stepNumber << endl;
        file << "Titlu: " << title << endl;
    }
};

class NumberInputStep : public Step {
private:
    string description;
    bool hasUserInput;  // Indică dacă input-ul a fost deja introdus
    float number_input;  // Input-ul specific pasului, setat ulterior

public:
    NumberInputStep(const string &d)
    {
        this->description = d;
        this->hasUserInput = false;
    }

    string getStepName() override {
        return "Number input step";
    }

    // Introduce descrierea pasului
    void inputDescription() {
        cout << "Introduceti descrierea: ";
        cin.ignore();  // Ignorăm orice caracter rămas în buffer
        getline(cin, description);
    }

    // Specifică input-ul pasului
    void specifyInput() {
        if (!hasUserInput) {
            cout << "Introduceti input-ul pentru pasul '" << description << "': ";
            cin >> number_input;
            hasUserInput = true;
        }
    }

    float getNumberInput() const {
        return number_input;
    }

    void getStepInfo() override {
        cout << "Descriere: " << description << endl;
        if (hasUserInput) {
            cout << "Input :" << number_input << endl;
        }
    }

    void writeToFile(ofstream &file) const {
        file << "NumberInputStep" << endl;
        file << "Descriere: " << description << endl;
        if (hasUserInput) {
            file << "Input: " << number_input << endl;
        }
    }
};

class CalculusStep : public Step {
private:
    int steps;
    string operation;
    vector<float> inputs;

public:
    CalculusStep(int s) : steps(s) {}

    float addition() const {
        float result = 0;
        for (float value : inputs) {
            result += value;
        }
        return result;
    }

    float subtraction() const {
        if (inputs.size() >= 2) {
            float result = inputs[0];
            for (size_t i = 1; i < inputs.size(); ++i) {
                result -= inputs[i];
            }
            return result;
        } else {
            cout << "Operatia de scadere necesita cel putin doua valori." << endl;
            return 0;
        }
    }

    float multiplication() const {
        float result = 1;
        for (float value : inputs) {
            result *= value;
        }
        return result;
    }

    float division() const {
        if (inputs.size() >= 2) {
            float result = inputs[0];
            for (size_t i = 1; i < inputs.size(); ++i) {
                if (inputs[i] != 0) {
                    result /= inputs[i];
                } else {
                    cout << "Impartirea la 0 nu este permisa." << endl;
                    return 0;
                }
            }
            return result;
        } else {
            cout << "Operatia de impartire necesita cel putin doua valori." << endl;
            return 0;
        }
    }

    float min() const {
        if (!inputs.empty()) {
            float result = inputs[0];
            for (size_t i = 1; i < inputs.size(); ++i) {
                result = std::min(result, inputs[i]);
            }
            return result;
        } else {
            cout << "Operatia de minim necesita cel putin o valoare." << endl;
            return 0;
        }
    }

    float max() const {
        if (!inputs.empty()) {
            float result = inputs[0];
            for (size_t i = 1; i < inputs.size(); ++i) {
                result = std::max(result, inputs[i]);
            }
            return result;
        } else {
            cout << "Operatia de maxim necesita cel putin o valoare." << endl;
            return 0;
        }
    }

    float performCalculation() const {
        if (operation == "+") {
            return addition();
        } else if (operation == "-") {
            return subtraction();
        } else if (operation == "*") {
            return multiplication();
        } else if (operation == "/") {
            return division();
        } else if (operation == "min") {
            return min();
        } else if (operation == "max") {
            return max();
        } else {
            cout << "Operatie necunoscuta." << endl;
            return 0;
        }
    }

    void specifyOperation() {
        cout << "Introduceti operatia pentru pasul 'Calculul' (+, -, *, /, min, max): ";
        cin >> operation;
    }

    void specifyInput() {
        cout << "Introduceti valorile pentru pasul 'Calculul':" << endl;
        for (int i = 0; i < steps; ++i) {
            float inputValue;
            cout << "Valoare pentru pasul " << (i + 1) << ": ";
            cin >> inputValue;
            inputs.push_back(inputValue);
        }
    }

    const vector<float>& getInputs() const {
        return inputs;
    }

    void getStepInfo() override {
        cout << "Operatie: " << operation << endl;
        cout << "Valori introduse:";
        for (const float &value : inputs) {
            cout << " " << value;
        }
        cout << endl;
    }

    string getStepName() override {
    return "Calculus step";
    }

    void writeToFile(ofstream &file) const {
        file << "CalculusStep" << endl;
        file << "Operatie: " << operation << endl;
        file << "Valori introduse:";
        for (const float &value : inputs) {
            file << " " << value;
        }
        file << endl;
    }
};

// Clasa principală pentru manipularea flow-urilor
class Flow {
private:
    string name;
    vector<Step *> steps;
    vector<string> stepNames;
    vector<float> number_Inputs;
    vector<float> inputs;
    bool hasEndStep;

public: 
    Flow(const string &n) : name(n) {
        hasEndStep = false;
    }

    string getFlowName() const {
        return name;
    }

    const vector<Step *> &getSteps() const {
        return steps;
    }

    void addStep(Step *newStep) {
    if (hasEndStep && dynamic_cast<EndStep *>(newStep)) {
        cout << "Un flow poate avea doar un End Step. Nu se poate adauga End Step suplimentar." << endl;
        delete newStep; // Eliberăm memoria alocată pentru EndStep
        return;
    }

    if (NumberInputStep *numberInputStep = dynamic_cast<NumberInputStep *>(newStep)) {
        number_Inputs.push_back(numberInputStep->getNumberInput());
    } else if (CalculusStep *calculusStep = dynamic_cast<CalculusStep *>(newStep)) {
        for (float value : calculusStep->getInputs()) {
            inputs.push_back(value);
        }
    } else if (dynamic_cast<EndStep *>(newStep)) {
        hasEndStep = true;
    }

    steps.push_back(newStep);
    stepNames.push_back(newStep->getStepName());
}

    const vector<float> &getNumberInputs() const {
        return number_Inputs;
    }

    void displayFlowInfo() const {
        cout << "Nume Flow: " << name << " | ";
        for (const string &stepName : stepNames) {
            cout << stepName << " | ";
        }
        cout << endl;
    }

    void runFlow() const {
        for (size_t i = 0; i < steps.size(); ++i) {
            cout << "Step " << (i + 1) << " of Flow '" << name << "':" << endl;
            steps[i]->getStepInfo();
            cout << endl;
        }
    }

    ~Flow() {
        for (Step *step : steps) {
            delete step;
        }
    }
};

class TitleStep: public Step
{
    private:
        string title, subtitle;
    public:
        TitleStep(const string &t, const string &s)
        {
            this->title = t;
            this->subtitle = s;
        }
        string getStepName() override
        {
            return "Title step";
        }
        void getStepInfo() override
        {
            cout << "Titlu: " << title << endl;
            cout << "Subtitlu: " << subtitle << endl;
        }

        void writeToFile(ofstream &file) const
        {
            file << "TitleStep" << endl;
            file << "Title: " << title << endl;
            file << "Subtitle: " << subtitle << endl;
        }
};

class TextStep: public Step
{
    private:
        string title, copy;
    public:
        TextStep(const string &t, const string &c)
        {
            this->title = t;
            this->copy = c;
        }
        string getStepName() override
        {
            return "Text step";
        }
        void getStepInfo() override
        {
            cout << "Titlu: " << title << endl;
            cout << "Copie: " << copy << endl;
        }
        void writeToFile(ofstream &file) const
        {
            file << "TextStep" << endl;
            file << "Title: " << title << endl;
            file << "Copie: " << copy << endl;
        }
};

class TextInputStep : public Step {
private:
    string description;
    bool hasUserInput;  // Indică dacă input-ul a fost deja introdus
    string text_input;  // Input-ul specific pasului, setat ulterior

public:
    TextInputStep(const string &d) {
        this->description = d;
        this->hasUserInput = false;
    }

    string getStepName() override {
        return "Text input step";
    }

    // Introduce descrierea pasului
    void inputDescription() {
        cout << "Introduceti descrierea: ";
        cin.ignore();  // Ignorăm orice caracter rămas în buffer
        getline(cin, description);
    }

    // Specifică input-ul pasului
    void specifyInput() {
        if (!hasUserInput) {
            cout << "Introduceti input-ul pentru pasul '" << description << "': ";
            cin.ignore();  // Ignorăm orice caracter rămas în buffer
            getline(cin, text_input);
            hasUserInput = true;
        }
    }

    string getTextInput() const {
        return text_input;
    }

    void getStepInfo() override {
        cout << "Descriere: " << description << endl;
        if (hasUserInput) {
            cout << "Input :" << text_input << endl;
        }
    }

    void writeToFile(ofstream &file) const
    {   
        file << "TextInputStep" << endl;
        file << "Descriere: " << description << endl;
        if (hasUserInput) {
            file << "Input: " << text_input << endl;
        }   
    }
};

class DisplayStep : public Step {
private:
    size_t stepNumber;

public:
    DisplayStep(size_t stepNum) : stepNumber(stepNum) {}

    string getStepName() override {
        return "Display Step";
    }

    void specifyStepNumber() {
        cout << "Introduceti numarul pasului pentru care doriti sa afisati continutul: ";
        cin >> stepNumber;
    }

    void getStepInfo() override {
        cout << "Numarul pasului pentru afisare: " << stepNumber << endl;
    }

    void displayContent(const vector<Step *> &steps) const {
        if (stepNumber >= 1 && stepNumber <= steps.size()) {
            if (TextFileInputStep *textFileStep = dynamic_cast<TextFileInputStep *>(steps[stepNumber - 1])) {
                displayFileContent(textFileStep->getFileName() + ".txt");
            } else if (CSVFileInputStep *csvFileStep = dynamic_cast<CSVFileInputStep *>(steps[stepNumber - 1])) {
                displayFileContent(csvFileStep->getFileName() + ".csv");
            } else if (OutputStep *outputStep = dynamic_cast< OutputStep *>(steps[stepNumber - 1])) {
                displayFileContent(outputStep->getFileName() + ".txt");
            } else {
                cout << "Pasul specificat nu este de tipul TextFileInputStep sau CSVFileInputStep." << endl;
            }
        } else {
            cout << "Numarul pasului specificat nu exista in flow!" << endl;
        }
    }

    void displayFileContent(const string &fileName) const {
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        // Am adăugat un bloc try-catch pentru a gestiona excepția în caz de eroare la deschiderea fișierului
        try {
            throw runtime_error("Eroare la deschiderea fisierului pentru afisare!");
        } catch (const exception &e) {
            cerr << e.what() << endl;
        }
    }
}

    void writeToFile(ofstream &file) const {
        file << "DisplayStep" << endl;
        file << "Numarul pasului pentru afisare: " << stepNumber << endl;
    }
};

int main() {
    vector<Flow> flows;
    int choice;

    do {
        cout << "\nMeniu:" << endl;
        cout << "1. Afiseaza lista de flow-uri" << endl;
        cout << "2. Creeaza un flow nou" << endl;
        cout << "3. Editeaza un flow existent" << endl;
        cout << "4. Sterge un flow" << endl;
        cout << "5. Iesire din aplicatie" << endl;
        cout << "Alege o optiune: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!flows.empty()) {
                    cout << "\nLista de flow-uri existente:" << endl;
                    for (const Flow &flow : flows) {
                        flow.displayFlowInfo();
                    }
                } else {
                    cout << "Nu exista flow-uri create." << endl;
                }
                break;
            case 2: {
                string flowName;
                cout << "Numele noului flow: ";
                cin >> flowName;

                string creationTime = getCurrentDateTime();
                cout << "Flow creat la: " << creationTime << endl;

                Flow newFlow(flowName + " - " + creationTime);

                vector<string> stepTypes = {"Title", "Text", "TextInput", "NumberInput", "Calculus", "TextFileInput", "CSVFileInput", "Output", "Display", "End"};

                for (const string &stepType : stepTypes) {
                    while (true) {
                        cout << "Doriti sa adaugati un step de tip " << stepType << " sau sa dati skip spre urmatorul step? (Introduceti da/skip): ";
                        string userChoice;
                        cin >> userChoice;

                        if (userChoice == "da") {
                            if (stepType == "Title") {
                                string title, subtitle;
                                cout << "Introduceti titlul: ";
                                cin >> title;
                                cout << "Introduceti subtitlul: ";
                                cin >> subtitle;
                                newFlow.addStep(new TitleStep(title, subtitle));
                            } else if (stepType == "Text") {
                                string title, copy;
                                cout << "Introduceti titlul: ";
                                cin >> title;
                                cout << "Introduceti copia: ";
                                cin >> copy;
                                newFlow.addStep(new TextStep(title, copy));
                            } else if (stepType == "TextInput") {
                                string description;
                                cout << "Introduceti descrierea: ";
                                cin.ignore();
                                getline(cin, description);
                                TextInputStep *textInputStep = new TextInputStep(description);
                                newFlow.addStep(textInputStep);
                            } else if (stepType == "NumberInput") {
                                NumberInputStep *numberInputStep = new NumberInputStep("");
                                numberInputStep->inputDescription();
                                newFlow.addStep(numberInputStep);
                            } else if (stepType == "Calculus") {
                                int numSteps;
                                cout << "Introduceti numarul de pasi pentru CalculusStep: ";
                                cin >> numSteps;
                                CalculusStep *calculusStep = new CalculusStep(numSteps);
                                newFlow.addStep(calculusStep);
                            } else if (stepType == "TextFileInput") {
                                string fileName;
                                cout << "Introduceti numele fisierului pentru TextFileInputStep: ";
                                cin >> fileName;
                                TextFileInputStep *textFileInputStep = new TextFileInputStep(fileName);
                                newFlow.addStep(textFileInputStep);
                                textFileInputStep->createFile();
                            } else if (stepType == "CSVFileInput") {
                                string fileName;
                                cout << "Introduceti numele fisierului pentru CSVFileInputStep: ";
                                cin >> fileName;
                                CSVFileInputStep *csvFileInputStep = new CSVFileInputStep(fileName);
                                newFlow.addStep(csvFileInputStep);
                                // După ce ai adăugat descrierea, poți să creezi și fișierul CSV
                                csvFileInputStep->createFile();
                            } else if (stepType == "Output") {
                                string fileName;
                                size_t stepNumber;
                                string title;
                                cout << "Introduceti numele fisierului pentru Output Step: ";
                                cin >> fileName;
                                cout << "Introduceti numarul step-ului din flow pentru care doriti informatiile: ";
                                cin >> stepNumber;
                                cout << "Introduceti titlul pentru prima linie din fisier: ";
                                cin.ignore();
                                getline(cin, title);

                                OutputStep *outputStep = new OutputStep(fileName, stepNumber, title);
                                newFlow.addStep(outputStep);
                                // Specifică informațiile și creează fișierul text
                                outputStep->createFile(newFlow.getSteps());
                            } else if (stepType == "Display") {
                                size_t stepNumber;
                                cout << "Introduceti numarul pasului pentru Display Step: ";
                                cin >> stepNumber;
                                DisplayStep *displayStep = new DisplayStep(stepNumber);
                                newFlow.addStep(displayStep);
                                // Afiseaza informatiile despre pas si continutul fisierului
                                displayStep->displayContent(newFlow.getSteps());
                            }   if (stepType == "End") {
                                    string userChoice;
                                    while (true) {
                                        cout << "Introduceti 'da' pentru a adauga EndStep si pentru a termina de creat flow-ul: ";
                                        cin >> userChoice;

                                        if (userChoice == "da") {
                                            EndStep *endStep = new EndStep();
                                            newFlow.addStep(endStep);
                                            break;  // Ieșim din buclă dacă utilizatorul a introdus "da"
                                        } else {
                                            cout << "Optiune invalida. 'da' este obligatoriu pentru a adauga EndStep." << endl;
                                        }
                                    }
                                    break;  // Ieșim din buclă pentru a trece la următorul flow
                                }
                            } else if (userChoice == "skip") {
                                break;  // Ieșim din buclă pentru a trece la următorul tip de pas
                            } else {
                                cout << "Optiune invalida. Te rog sa reintroduci optiunea." << endl;
                            }
                        }
                    }

                flows.push_back(newFlow);

                cout << "Flow creat cu succes!" << endl;
                break;
            }
            case 3:
                if (!flows.empty()) {
                    cout << "Alege un flow pentru editare (1-" << flows.size() << "): ";
                    size_t flowIndex;
                    cin >> flowIndex;

                    if (flowIndex >= 1 && flowIndex <= flows.size()) {
                        cout << "Editare flow " << flows[flowIndex - 1].getFlowName() << endl;
                    } else {
                        cout << "Alegere invalida." << endl;
                    }
                } else {
                    cout << "Nu exista flow-uri create." << endl;
                }
                break;
            case 4:
                if (!flows.empty()) {
                    cout << "Alege un flow pentru stergere (1-" << flows.size() << "): ";
                    size_t flowIndex;
                    cin >> flowIndex;

                    if (flowIndex >= 1 && flowIndex <= flows.size()) {
                        // Construim un nou vector care exclude flow-ul pe care dorim să-l ștergem
                        vector<Flow> newFlows;
                        for (size_t i = 0; i < flows.size(); ++i) {
                            if (i != flowIndex - 1) {
                                newFlows.push_back(flows[i]);
                            }
                        }
                        // Schimbăm vectorul vechi cu cel nou
                        flows = newFlows;
                        
                        cout << "Flow sters cu succes!" << endl;
                    } else {
                        cout << "Alegere invalida." << endl;
                    }
                } else {
                    cout << "Nu exista flow-uri create." << endl;
                }
                break;
            case 5:
                for (Flow &flow : flows) {
                    flow.runFlow();
                }
                cout << "Iesire din aplicatie. La revedere!" << endl;
                break;
            default:
                cout << "Optiune invalida. Te rog sa reintroduci optiunea." << endl;
                break;
        }
    } while (choice != 5);

    return 0;
}