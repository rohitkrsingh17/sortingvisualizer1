#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int NUM_BARS = 128;
const int BAR_WIDTH = SCREEN_WIDTH / NUM_BARS;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }
    window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderBars(const vector<int>& bars, int highlight1 = -1, int highlight2 = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < bars.size(); ++i) {
        if (i == highlight1 || i == highlight2) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_Rect bar = {i * BAR_WIDTH, SCREEN_HEIGHT - bars[i], BAR_WIDTH, bars[i]};
        SDL_RenderFillRect(renderer, &bar);
    }

    SDL_RenderPresent(renderer);
}

void bubbleSort(vector<int>& bars) {
    for (int i = 0; i < bars.size() - 1; ++i) {
        for (int j = 0; j < bars.size() - i - 1; ++j) {
            if (bars[j] > bars[j + 1]) {
                swap(bars[j], bars[j + 1]);
                renderBars(bars, j, j + 1);
                usleep(1000); // Faster animation
            }
        }
    }
}

void selectionSort(vector<int>& bars) {
    for (int i = 0; i < bars.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < bars.size(); ++j) {
            if (bars[j] < bars[minIndex]) {
                minIndex = j;
            }
        }
        swap(bars[i], bars[minIndex]);
        renderBars(bars, i, minIndex);
        usleep(5000); // Faster animation
    }
}

void insertionSort(vector<int>& bars) {
    for (int i = 1; i < bars.size(); ++i) {
        int key = bars[i];
        int j = i - 1;
        while (j >= 0 && bars[j] > key) {
            bars[j + 1] = bars[j];
            renderBars(bars, j, j + 1);
            usleep(1000); // Faster animation
            --j;
        }
        bars[j + 1] = key;
        renderBars(bars, j + 1);
        usleep(1000); // Faster animation
    }
}

void merge(vector<int>& bars, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = bars[l + i];
    for (int i = 0; i < n2; ++i) R[i] = bars[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            bars[k] = L[i];
            ++i;
        } else {
            bars[k] = R[j];
            ++j;
        }
        renderBars(bars, k);
        usleep(1000); // Faster animation
        ++k;
    }
    while (i < n1) {
        bars[k] = L[i];
        renderBars(bars, k);
        usleep(1000); // Faster animation
        ++i;
        ++k;
    }
    while (j < n2) {
        bars[k] = R[j];
        renderBars(bars, k);
        usleep(1000); // Faster animation
        ++j;
        ++k;
    }
}

void mergeSort(vector<int>& bars, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(bars, l, m);
    mergeSort(bars, m + 1, r);
    merge(bars, l, m, r);
}

int partition(vector<int>& bars, int low, int high) {
    int pivot = bars[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; ++j) {
        if (bars[j] < pivot) {
            ++i;
            swap(bars[i], bars[j]);
            renderBars(bars, i, j);
            usleep(1000); // Faster animation
        }
    }
    swap(bars[i + 1], bars[high]);
    renderBars(bars, i + 1, high);
    usleep(1000); // Faster animation
    return i + 1;
}

void quickSort(vector<int>& bars, int low, int high) {
    if (low < high) {
        int pi = partition(bars, low, high);
        quickSort(bars, low, pi - 1);
        quickSort(bars, pi + 1, high);
    }
}

void heapify(vector<int>& bars, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && bars[left] > bars[largest]) largest = left;
    if (right < n && bars[right] > bars[largest]) largest = right;
    if (largest != i) {
        swap(bars[i], bars[largest]);
        renderBars(bars, i, largest);
        usleep(1000); // Faster animation
        heapify(bars, n, largest);
    }
}

void heapSort(vector<int>& bars) {
    for (int i = bars.size() / 2 - 1; i >= 0; --i) heapify(bars, bars.size(), i);
    for (int i = bars.size() - 1; i > 0; --i) {
        swap(bars[0], bars[i]);
        renderBars(bars, 0, i);
        usleep(1000); // Faster animation
        heapify(bars, i, 0);
    }
}

int main(int argc, char* args[]) {
    initSDL();
    srand(time(0));

    vector<int> bars(NUM_BARS);
    for (int i = 0; i < NUM_BARS; ++i) bars[i] = (rand() % (SCREEN_HEIGHT - 100)) + 50;

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_b:
                        bubbleSort(bars);
                        break;
                    case SDLK_s:
                        selectionSort(bars);
                        break;
                    case SDLK_i:
                        insertionSort(bars);
                        break;
                    case SDLK_m:
                        mergeSort(bars, 0, bars.size() - 1);
                        break;
                    case SDLK_q:
                        quickSort(bars, 0, bars.size() - 1);
                        break;
                    case SDLK_h:
                        heapSort(bars);
                        break;
                }
            }
        }
        renderBars(bars);
    }

    closeSDL();
    return 0;
}
