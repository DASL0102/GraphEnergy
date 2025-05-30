#include <SFML/Graphics.hpp>
#include <iostream>


class DynamicForm {
        private:
            sf::RectangleShape background;
            std::vector<sf::Text> labels;
            std::vector<sf::RectangleShape> inputFields;
            std::vector<sf::Text> inputTexts;
            sf::RectangleShape submitButton;
            sf::Text submitButtonText;
            sf::Text infoSubmitText;
            sf::Text titleText;
            sf::Text closeText;
            std::string infoSubmitMessage;
            bool isVisible;
            sf::Font font;
            int activeField = -1;
            std::string currentFormType;
            
        public:
            DynamicForm(float width, float height, float posX, float posY);
            
            void clearFields() ;
            
            void addField(const std::string& label, float yOffset);

            void setinfoSubmitText(std::string message);

            void createForm(const std::string& formType, sf::RenderWindow& window);

            bool isFormFilled(const sf::Event& event, sf::RenderWindow& window, const std::string  formType);

            bool handleEvent(const sf::Event& event, sf::RenderWindow& window) ;

            bool handleEventJsonMenu(const sf::Event& event, sf::RenderWindow& window);

            void show() ;
            void hide() ;
            void toggle() ;
            bool visible() const ;
            std::string getFormType() const ;
            
            void draw(sf::RenderWindow& window) ;
            // Función para obtener los datos ingresados
            std::vector<std::string> getFormData() const ;
            bool isNumeric(const std::string& str); 
};
        