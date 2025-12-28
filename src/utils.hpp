//
// Created by remza on 28.12.2025.
//

#pragma once


//SDL_Window *window = nullptr;
//SDL_Renderer *renderer = nullptr;
//
//SDL_SetAppMetadata(filename.c_str(), "1.0.0", "PNG Viewer");
//
//if (!SDL_Init(SDL_INIT_VIDEO) != 0) {
//Logger::error() << "SDL_Init Error: " << SDL_GetError() << std::endl;
//std::exit(EXIT_FAILURE);
//}
//
//if (!SDL_CreateWindowAndRenderer(filename.c_str(), hdr_data.width, hdr_data.height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
//SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
//return SDL_APP_FAILURE;
//}
//
//SDL_SetRenderLogicalPresentation(renderer, hdr_data.width, hdr_data.height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
//
//SDL_FRect rect;
//int scale = 1;
//rect.w = scale;
//rect.h = scale;
//
//for (uint32_t y = 0; y < hdr_data.height; y++) {
//const uint8_t filter_type = pixels[y * (hdr_data.width * 3 + 1)];
//
//for (uint32_t x = 0; x < hdr_data.width; x++) {
//const uint8_t r = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 0];
//const uint8_t g = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 1];
//const uint8_t b = pixels[y * (hdr_data.width * 3 + 1) + 1 + x * 3 + 2];
//
//SDL_SetRenderDrawColor(renderer, r, g, b, 255);
//
//rect.x = x * scale; // position scaled
//rect.y = y * scale;
//SDL_RenderFillRect(renderer, &rect);
//}
//}
//
//SDL_RenderPresent(renderer);
//
//for (;;) {
//SDL_Event event;
//while (SDL_PollEvent(&event)) {
//if (event.type == SDL_EVENT_QUIT) {
//SDL_DestroyRenderer(renderer);
//SDL_DestroyWindow(window);
//SDL_Quit();
//return SDL_APP_SUCCESS;
//}
//}
//
//}
//
//return SDL_APP_CONTINUE;