#pragma once

#include <bits/stdc++.h>
#include <optional>
#include <SFML/Graphics.hpp>

#include "gapbuffer.hpp"
#include "caret.hpp"
#include "selectionbox.hpp"
#include "scroll_view.hpp"
#include "helpers.hpp"

using namespace std;

class Screen {
public:
    Screen(const string& contents = "");

    void set_on_save(std::function<void(const string&)> callback) {
        on_save = callback;
    }

    void run_window();


private:

    void handle_events();

    void on_key_pressed(const sf::Event::KeyPressed* keyPressed);
    void handle_commands(const sf::Event::KeyPressed* keyPressed);
    void handle_arrow_keys(const sf::Event::KeyPressed* keyPressed);

    void type_char(char32_t c);

    // Filter out the characters that the text editor allows. Also handle return by using '\n'
    // instead of '\r'.
    const std::optional<char32_t> get_valid_char(int unicode) const;

    void delete_selection();

    // Set sf::Text string to buffer text, then update the scroll view content size
    void update_text() {

        text.setString(buffer.get_display_str());

        sf::Vector2f padding = { 28, 28 };
        sf::Vector2f content_size = Helpers::find_text_area_size(buffer.get_display_str()) + padding;

        scroll_view.set_content_size((sf::Vector2i)content_size);
    }

    int get_num_lines() {
        const string& str = buffer.get_display_str();
        int return_count = count(str.begin(), str.end(), '\n');
        int num_lines = return_count + 1;
        return num_lines;
    }

    void ensure_caret_visible(sf::Vector2f char_pos) {
        sf::Vector2f char_center = char_pos + (TEXT_SHAPE_OFFSET / 2.f) + sf::Vector2f(CHARACTER_WIDTH / 2.f, LINE_HEIGHT / 2.f);
        sf::Vector2f padding = { CHARACTER_WIDTH * 1.5, LINE_HEIGHT * 2.5 };
        scroll_view.scroll_to_show_pos(char_center, padding);
    }

    std::function<void(const string&)> on_save;

    GapBuffer buffer;

    sf::Font font;

    // Includes the whole text. If I want to optimize, then I would only include the visible
    // section of this text and move it based on the scroll. window.draw(text) is costly when
    // text is large.
    sf::Text text;

    Caret caret;

    SelectionBox selection_box;
    int selection_start_index;

    sf::RenderWindow window;

    ScrollView scroll_view;

    bool debug_print_draw_time;
};