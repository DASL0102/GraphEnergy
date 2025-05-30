#include "DynamicForm.hpp"

const int FIX_POS_X2 = 500; 
const int FIX_POS_Y2 = 350; 

const int FIX_FORM_POS_X = 150; 
const int FIX_FORM_POS_Y = 150; 


DynamicForm::DynamicForm(float width, float height, float posX, float posY) {
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(posX, posY);
    background.setFillColor(sf::Color(230, 230, 230, 220)); 
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));
    
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
    }
    
    submitButton.setSize(sf::Vector2f(100, 30));
    submitButton.setFillColor(sf::Color(30, 180, 30));
    submitButtonText.setFont(font);
    submitButtonText.setString("Confirmar");
    submitButtonText.setCharacterSize(14);
    submitButtonText.setFillColor(sf::Color::White);
    
    
    infoSubmitMessage = "Rellena todos los campos.";

    //Message for the user fields
    infoSubmitText.setFont(font);
    infoSubmitText.setString(infoSubmitMessage);
    infoSubmitText.setCharacterSize(14);
    infoSubmitText.setFillColor(sf::Color::Black);

    //Title form
    titleText.setFont(font);
    titleText.setString("Agregar Estructura");
    titleText.setCharacterSize(16);
    titleText.setFillColor(sf::Color::Black);

    //Close info
    closeText.setFont(font);
    closeText.setString("(Presiona X para Cancelar)");
    closeText.setCharacterSize(10);
    closeText.setFillColor(sf::Color::Red);


    isVisible = false;
}

void DynamicForm::clearFields() {
    labels.clear();
    inputFields.clear();
    inputTexts.clear();
    activeField = -1;
}

void DynamicForm::addField(const std::string& label, float yOffset) {
    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setPosition(background.getPosition().x + 15, 
                     background.getPosition().y + yOffset);
    labels.push_back(text);
    
    sf::RectangleShape field(sf::Vector2f(background.getSize().x - 30, 25));
    field.setPosition(background.getPosition().x + 15, 
                      background.getPosition().y + yOffset + 25);
    field.setFillColor(sf::Color::White);
    field.setOutlineThickness(1);
    field.setOutlineColor(sf::Color(150, 150, 150));
    inputFields.push_back(field);
    
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(16);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(field.getPosition().x + 5, field.getPosition().y + 2);
    inputTexts.push_back(inputText);
}

void DynamicForm::createForm(const std::string& formType, sf::RenderWindow& window ) {
    clearFields();


    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    background.setPosition(mousePos.x - FIX_FORM_POS_X, mousePos.y - FIX_FORM_POS_Y);

    if(formType == "Grabar Doc." or formType == "Cargar Doc." or formType == "Eliminar" or formType == "Eliminar Nodo." or formType == "E. Nodo"){
            background.setPosition(mousePos.x - FIX_FORM_POS_X, mousePos.y - FIX_FORM_POS_Y + 200);
    }
    if( formType == "E. Nodo"){
            background.setPosition(mousePos.x - FIX_FORM_POS_X - 100, mousePos.y - FIX_FORM_POS_Y + 200);
    }

    const int POS_MESSAGE_X = 20;
    const int POS_MESSAGE_Y = 200;

    int POS_TITLE_X = 60;
    const int POS_TITLE_Y = 20;

    const int POS_CLOSE_MESSAGE_X = -25;
    const int POS_CLOSE_MESSAGE_Y = 35;

    const float REFERENCE_POS_X = background.getPosition().x;
    const float REFERENCE_POS_Y = background.getPosition().y;




    currentFormType = formType;
    
    infoSubmitText.setString(infoSubmitMessage);

    if (formType == "Casa") {

        titleText.setString("Agregar Casa");
        POS_TITLE_X += 20;
        addField("Nombre:", 55);
        addField("Consumo Estimado (kW):", 120);
    }
    else if (formType == "Subestacion") {

        titleText.setString("Agregar Subestacion");
        addField("Nombre:", 55);
        addField("Capacidad Maxima:", 120);
    }
    else if (formType == "Estacion") {
        POS_TITLE_X += 15;

        titleText.setString("Agregar Estacion");
        addField("Nombre:", 55);
        addField("Capacidad Maxima:", 120);

    }else if(formType == "Grabar Doc.") {

        POS_TITLE_X += 15;
        titleText.setString("Grabar Archivo");
        addField("Nombre del archivo", 70);
    }else if(formType == "Cargar Doc.") {

        POS_TITLE_X += 15;
        titleText.setString("Cargar Archivo");
        addField("Nombre del archivo", 70);
    }else if(formType == "Eliminar") {

        POS_TITLE_X += 15;
        titleText.setString("Eliminar Nodo");
        addField("Id del Nodo", 70);
    }
    else if(formType == "E. Nodo") {

        POS_TITLE_X += 15;
        titleText.setString("Eliminar Nodo");
        addField("Id del Nodo", 70);
    }


    //Ajustar posición del botón de envío
    float lastFieldY = background.getPosition().y + 120 + 25 + 20;
    submitButton.setPosition(background.getPosition().x + background.getSize().x/2 - 50, lastFieldY + 80);
    submitButtonText.setPosition(submitButton.getPosition().x + 10, submitButton.getPosition().y + 8 );

    infoSubmitText.setPosition(REFERENCE_POS_X  + POS_MESSAGE_X, REFERENCE_POS_Y + POS_MESSAGE_Y );

    titleText.setPosition(REFERENCE_POS_X  + POS_TITLE_X, REFERENCE_POS_Y + POS_TITLE_Y );

    closeText.setPosition(submitButton.getPosition().x  + POS_CLOSE_MESSAGE_X, submitButton.getPosition().y  + POS_CLOSE_MESSAGE_Y );

    show();
}

bool DynamicForm::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!isVisible) return false;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            sf::Vector2f mouseOne = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            //sf::Vector2i mouseOne1 = sf::Mouse::getPosition(window);
            //sf::Vector2f mouseOne(mouseOne1.x, mouseOne1.y); 


            activeField = -1;
            for (size_t i = 0; i < inputFields.size(); ++i) {

                if (inputFields[i].getGlobalBounds().contains(mouseOne)) {
                    activeField = static_cast<int>(i);
                    inputFields[i].setOutlineColor(sf::Color::Blue);
                    inputFields[i].setOutlineThickness(2);
                } else {
                    inputFields[i].setOutlineColor(sf::Color(150, 150, 150));
                    inputFields[i].setOutlineThickness(1);
                }
            }
            
            if (submitButton.getGlobalBounds().contains(mouseOne)) {
                bool allFieldsFilled = true;
                int j = 0;
                for (const auto& text : inputTexts) {
                    if (text.getString().isEmpty()) {
                        allFieldsFilled = false;
                        infoSubmitText.setString("Debes llenar todos los campos.");

                        break;
                    }

                    //Check is the numeric field is positive and it does not has strange chars
                    if(j == 1){
                        bool flag = isNumeric(text.getString());

                        if(!flag){
                            allFieldsFilled = false;
                            inputFields[j].setOutlineColor(sf::Color::Red);
                            infoSubmitText.setString("Agrega un numero positivo valido.");

                            break;
                        }
                    }
                    j++;
                }
                
                if (allFieldsFilled) {
                    hide(); 
                    return true; 
                } else {
                    // Resaltar campos vacíos
                    for (size_t i = 0; i < inputTexts.size(); ++i) {
                        if (inputTexts[i].getString().isEmpty()) {
                            inputFields[i].setOutlineColor(sf::Color::Red);
                            inputFields[i].setOutlineThickness(2);
                        }
                    }
                }
            }
        }
    }
    
    if (event.type == sf::Event::TextEntered && activeField != -1) {
        if (event.text.unicode == '\b') { 
            if (!inputTexts[activeField].getString().isEmpty()) {
                std::string str = inputTexts[activeField].getString();
                str.pop_back();
                inputTexts[activeField].setString(str);
            }
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
            std::string str = inputTexts[activeField].getString();
            str += static_cast<char>(event.text.unicode);
            inputTexts[activeField].setString(str);
        }
    }
    
    return false;
}


bool DynamicForm::handleEventJsonMenu(const sf::Event& event, sf::RenderWindow& window) {
    if (!isVisible) return false;
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouseOne = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            //sf::Vector2i mouseOne1 = sf::Mouse::getPosition(window);
            //sf::Vector2f mouseOne(mouseOne1.x, mouseOne1.y); 


            activeField = -1;
            for (size_t i = 0; i < inputFields.size(); ++i) {

                if (inputFields[i].getGlobalBounds().contains(mouseOne)) {
                    activeField = static_cast<int>(i);
                    inputFields[i].setOutlineColor(sf::Color::Blue);
                    inputFields[i].setOutlineThickness(2);
                } else {
                    inputFields[i].setOutlineColor(sf::Color(150, 150, 150));
                    inputFields[i].setOutlineThickness(1);
                }
            }
            
            if (submitButton.getGlobalBounds().contains(mouseOne)) {
                bool allFieldsFilled = true;
                for (const auto& text : inputTexts) {
                    if (text.getString().isEmpty()) {
                        allFieldsFilled = false;
                        infoSubmitText.setString("Agrega un nombre de archivo.");
                        break;
                    }
                }
                
                if (allFieldsFilled) {
                    hide(); 
                    return true; 
                } else {
                    // Resaltar campos vacíos
                    for (size_t i = 0; i < inputTexts.size(); ++i) {
                        if (inputTexts[i].getString().isEmpty()) {
                            inputFields[i].setOutlineColor(sf::Color::Red);
                            inputFields[i].setOutlineThickness(2);
                        }
                    }
                }
            }
        }
    }
    
    if (event.type == sf::Event::TextEntered && activeField != -1) {
        if (event.text.unicode == '\b') { 
            if (!inputTexts[activeField].getString().isEmpty()) {
                std::string str = inputTexts[activeField].getString();
                str.pop_back();
                inputTexts[activeField].setString(str);
            }
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
            std::string str = inputTexts[activeField].getString();
            str += static_cast<char>(event.text.unicode);
            inputTexts[activeField].setString(str);
        }
    }
    
    return false;
}



bool DynamicForm::isFormFilled(const sf::Event& event, sf::RenderWindow& window, const std::string  formType ) {
    if(formType == "Grabar Doc." or formType == "Cargar Doc."){
        return handleEventJsonMenu(event, window);
    }else{
        return handleEvent(event, window);
    }
}

void DynamicForm::show() { isVisible = true; }

void DynamicForm::hide() { isVisible = false; }

void DynamicForm::toggle() { isVisible = !isVisible; }

bool DynamicForm::visible() const { return isVisible; }

std::string DynamicForm::getFormType() const { return currentFormType; }

void DynamicForm::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    
    window.draw(background);
    
    for (auto& label : labels) {
        window.draw(label);
    }
    
    for (auto& field : inputFields) {
        window.draw(field);
    }
    
    for (auto& text : inputTexts) {
        window.draw(text);
    }
    
    window.draw(submitButton);
    window.draw(submitButtonText);
    window.draw(infoSubmitText);
    window.draw(titleText);
    window.draw(closeText);
    
}


std::vector<std::string> DynamicForm::getFormData() const {
    std::vector<std::string> data;
    for (const auto& text : inputTexts) {
        data.push_back(text.getString());
    }
    return data;
}


bool DynamicForm::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    

   
        if(str[0] == '-'){
            if (str[1] >= '0' && str[1] <= '9') {
                return false;
            }
           
        }
        try {
            std::string eval;
            for(auto c : str){
                if(c != ' ' and c != '\n'){
                    if(c >= '0' and c <= '9'){
                        eval.push_back(c);
                    }else if(c == '.'){
                        eval.push_back(c);
                    }else{
                        return false;
                    }
                }
            }
            float numero = std::stof(eval); // Intenta convertir a float
            std::cout << "Número convertido: " << numero << std::endl;
            if(numero < 0){
                return false;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: No es un número flotante válido." << std::endl;
            // Aquí puedes hacer algo cuando falla la conversión
            return false;

        }
       return true;
}

void DynamicForm::setinfoSubmitText(std::string message){
    infoSubmitText.setString(message);
}
