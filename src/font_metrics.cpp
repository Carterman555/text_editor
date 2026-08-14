#include "font_metrics.hpp"
#include <cassert>

namespace {
    FontMetrics::Metrics g_metrics;
    bool g_initialized = false;
}

const FontMetrics::Metrics& FontMetrics::get() {
    assert(g_initialized && "Font metrics were used before initialized");
    return g_metrics;
}

void FontMetrics::init(const sf::Font& font, unsigned int font_size) {
    g_metrics.line_height = font.getLineSpacing(font_size);
    g_metrics.char_width = font.getGlyph('x', font_size, false).advance;
    g_metrics.tab_width = g_metrics.char_width * 4.f;
    g_initialized = true;
}
