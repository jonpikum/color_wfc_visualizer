
#include "./visualizer.hpp"
#include "imgui.h"

ColorWFCVisualizer::ColorWFCVisualizer() : _counterIndex(s_counter++) {}

ColorWFCVisualizer::~ColorWFCVisualizer() {}

void ColorWFCVisualizer::Draw(VisualizationType displayType,
  std::vector<std::vector<ColorTileCandidate *>> &grid, uint8_t tileDrawSize,
  ColorTileCandidate *&hoveredCandidate, uint32_t totalColorPossibilities) {

  if (displayType == VisualizationType::Average) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
  }

  std::string title(
    "WFC Block Visualizer #" + std::to_string(this->_counterIndex));
  ImGui::Begin(title.c_str());

  if (displayType == VisualizationType::Average) {
    ImGui::PopStyleColor(1);
  }

  auto drawList = ImGui::GetWindowDrawList();

  ImVec2 p = ImGui::GetCursorScreenPos();

  for (size_t k = 0; k < grid.at(0).size(); k++) {
    for (size_t i = 0; i < grid.size(); i++) {
      auto ctc = grid.at(i).at(k);
      if (ctc == nullptr) {
        continue;
      }

      ImVec2 markerMin =
        ImVec2(p.x + (i * tileDrawSize), p.y + (k * tileDrawSize));
      ImVec2 markerMax =
        ImVec2(markerMin.x + tileDrawSize, markerMin.y + tileDrawSize);

      if (ImGui::IsMouseHoveringRect(markerMin, markerMax)) {
        hoveredCandidate = ctc;
      }

      ImVec4 color;
      ColorTile *ct = ctc->GetValue();
      if (displayType == VisualizationType::Color) {
        if (ct != nullptr) {
          color =
            ImVec4(ct->r / 255.f, ct->g / 255.f, ct->b / 255.f, ct->a / 255.f);
        }
      } else if (displayType == VisualizationType::Entropy) {
        auto entropyRatio = ctc->GetEntropy() / (float)totalColorPossibilities;
        color = ImVec4(entropyRatio, entropyRatio, entropyRatio, 1.f);
      } else if (displayType == VisualizationType::Average) {
        auto avgColor = ctc->GetAverageColor();
        color = ImVec4(avgColor.r / 255.f, avgColor.g / 255.f,
          avgColor.b / 255.f, avgColor.a / 255.f);
      }
      drawList->AddRectFilled(markerMin, markerMax, ImColor(color));
    }
  }

  ImGui::End();
}
