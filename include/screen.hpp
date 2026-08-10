#pragma once

#include <bits/stdc++.h>
#include <optional>
#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "caret.hpp"
#include "selectionbox.hpp"
#include "scroll_view.hpp"
#include "helpers.hpp"
#include "event_manager.hpp"
#include "warning_popup.hpp"

using namespace std;

class Screen {
public:
    Screen(const string& contents = "");

    void set_on_save(std::function<void(const string&)> callback) {
        on_save = callback;
    }

    // start the update loop which runs the text editor
    void run_window();

private:

    // check whether the given string contains char values that the text editor does not support
    bool contains_non_ascii(const string& str);

    void handle_events();
    void handle_commands(const Event& event);
    void handle_arrow_keys(const Event& event);

    /**
     * @brief select the group of the based on the character after the given pos
     *
     * 3 Groups: letters + numbers; symbols; whitespace
     *
     * Determines which group the character after the caret is a part of, then select the adjacent
     * characters in that group. If the caret is at the end of the text, just clear the selection.
     */
    void select_group(int pos);

    // select the line the given pos is a part of
    void select_line(int pos);

    // insert a character at the caret pos and move the caret pos right by one
    void type_char(char c);

    // type a sequence of characters at the caret given
    void type_sequence(const string& str);

    void delete_selection();

    // Set sf::Text string to buffer text, then update the scroll view content size
    void update_text() {

        text.setString(buffer.get_display_str());

        sf::Vector2f padding = { 28, 28 };
        sf::Vector2f content_size = Helpers::find_text_area_size(buffer.get_display_str()) + padding;

        scroll_view.set_content_size((sf::Vector2i)content_size);
    }

    /**
     * @brief calculate the char index of the mouse position
     *
     * @param local_mouse_pixel_pos Must be the mouse position relative to the top left corner of
     * the window. The global mouse position won't work.
     */
    int mouse_text_pos(sf::Vector2i local_mouse_pixel_pos) {
        sf::Vector2f mouse_pos = window.mapPixelToCoords(local_mouse_pixel_pos, scroll_view.get_content_view());
        int pos = Helpers::pos_to_char_index(buffer.get_display_str(), mouse_pos);
        return pos;
    }

    int get_num_lines() {
        const string& str = buffer.get_display_str();
        int return_count = count(str.begin(), str.end(), '\n');
        int num_lines = return_count + 1;
        return num_lines;
    }

    // scroll the screen to ensure the caret is visible in the scroll view (with padding)
    void ensure_caret_visible(sf::Vector2f char_pos) {
        sf::Vector2f char_center = char_pos + (TEXT_SHAPE_OFFSET / 2.f) + sf::Vector2f(CHARACTER_WIDTH / 2.f, LINE_HEIGHT / 2.f);
        sf::Vector2f padding = { CHARACTER_WIDTH * 1.5f, LINE_HEIGHT * 2.5f };
        scroll_view.scroll_to_show_pos(char_center, padding);
    }

    std::function<void(const string&)> on_save;

    GapBuffer buffer;

    sf::Font font{ (std::string)PROJECT_DIR + "/CONSOLA.TTF" };

    // Includes the whole text. If I want to optimize, then I would only include the visible
    // section of this text and move it based on the scroll. window.draw(text) is costly when
    // text is large.
    sf::Text text{ font };

    Caret caret{ buffer };

    SelectionBox selection_box{ buffer };
    int selection_start_index = 0;

    sf::RenderWindow window{ sf::VideoMode(WINDOW_SIZE), "Text Editor" };

    ScrollView scroll_view{ window };

    EventManager event_manager{ window };

    WarningPopup warning_popup;
};