#pragma once
#include "../app.hpp"
#include <ftxui/component/component.hpp>

// Builds and returns the full interactive component tree.
// `state` is mutated by the components as the user interacts.
ftxui::Component BuildUI(App &state);
