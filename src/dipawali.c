#include "raylib.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NUM_DIYAS 3
#define MAX_FIREWORKS 10
#define MAX_PARTICLES 200
#define MAX_TEXT_SPARKLES 50
#define MAX_SPARKLER_TRAIL 100

// Firework Types
typedef enum {
    FIREWORK_CRACKLE_ROCKET,
    FIREWORK_FOUNTAIN,
    FIREWORK_BURST,
} FireworkType;

typedef struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifespan;
    bool active;
    bool is_secondary; // For crackle effect
} Particle;

typedef struct {
    FireworkType type;
    Vector2 position;
    Vector2 velocity;
    Color color;
    bool exploded;
    int particle_count;
    Particle particles[MAX_PARTICLES];
} Firework;

typedef struct {
    Vector2 position;
} Diya;

typedef struct {
    char character[2];
    Vector2 position;
    Vector2 velocity;
    float final_y;
    float alpha;
    float rotation;
    Color color;
    bool active;
    float brightness;
} AnimatedLetter;

typedef struct {
    Vector2 position;
    int target_letter;
    float speed;
    bool active;
    Color color;
} Sparkler;

void DrawDiya(Diya diya, int index) {
    // Pot
    DrawCircleGradient(diya.position.x, diya.position.y, 30, (Color){ 180, 80, 20, 255 }, (Color){ 100, 40, 10, 255 });
    DrawCircle(diya.position.x, diya.position.y - 5, 25, (Color){ 50, 20, 5, 255 });

    // Wick
    DrawRectangle(diya.position.x - 2, diya.position.y - 35, 4, 10, DARKBROWN);

    // New Flame Logic
    float time = GetTime();
    float base_width = 15.0f;
    float base_height = 30.0f;

    // Use index to create a unique offset for each flame
    float width_flicker = sinf(time * 2.0f + index * 2.5f) * 2.0f;
    float height_flicker = sinf(time * 3.0f + index * 3.0f) * 3.5f;
    float core_flicker = sinf(time * 1.5f + index * 2.0f) * 1.5f;

    // Outer Orange Flame
    DrawEllipse(diya.position.x, diya.position.y - 55 - height_flicker*0.5f, base_width + width_flicker, base_height + height_flicker, ORANGE);
    // Inner Yellow Flame
    DrawEllipse(diya.position.x, diya.position.y - 53 - core_flicker*0.5f, (base_width/2) + core_flicker, (base_height/2) + core_flicker, YELLOW);
}

void InitFirework(Firework *fw, int index) {
    fw->exploded = false;
    fw->particle_count = 0;

    // Dedicate two fireworks as fountains on the sides
    if (index < 2) { 
        fw->type = FIREWORK_FOUNTAIN;
    } else { // The rest are aerial
        int type_selector = GetRandomValue(0, 1);
        if (type_selector == 0) {
            fw->type = FIREWORK_BURST;
        } else {
            fw->type = FIREWORK_CRACKLE_ROCKET;
        }
    }

    switch (fw->type) {
        case FIREWORK_BURST:
            fw->position = (Vector2){ (float)GetRandomValue(200, SCREEN_WIDTH - 200), (float)SCREEN_HEIGHT };
            fw->velocity = (Vector2){ (float)GetRandomValue(-2, 2), (float)GetRandomValue(-8, -5) };
            fw->color = (Color){ GetRandomValue(100, 255), GetRandomValue(100, 255), GetRandomValue(100, 255), 255 };
            break;
        case FIREWORK_FOUNTAIN:
            fw->position = (Vector2){ (index == 0) ? 100.0f : SCREEN_WIDTH - 100.0f, (float)SCREEN_HEIGHT - 20 };
            fw->velocity = (Vector2){ 0, 0 };
            fw->color = (Color){ GetRandomValue(200, 255), GetRandomValue(50, 150), 0, 255 };
            fw->exploded = true;
            break;
        case FIREWORK_CRACKLE_ROCKET:
            fw->position = (Vector2){ (float)GetRandomValue(200, SCREEN_WIDTH - 200), (float)SCREEN_HEIGHT };
            fw->velocity = (Vector2){ (float)GetRandomValue(-1, 1), (float)GetRandomValue(-8, -5) };
            fw->color = (Color){ GetRandomValue(200, 255), GetRandomValue(200, 255), 200, 255 };
            break;
    }

    for (int i = 0; i < MAX_PARTICLES; i++) {
        fw->particles[i].active = false;
    }
}

void UpdateFirework(Firework *fw, float dt, int index) {
    if (!fw->exploded) {
        fw->position.x += fw->velocity.x * dt * 60;
        fw->position.y += fw->velocity.y * dt * 60;
        fw->velocity.y += 0.05f * dt * 60;

        if (fw->velocity.y >= 0) {
            fw->exploded = true;
            fw->particle_count = 0;
            int num_particles = 0;

            switch (fw->type) {
                case FIREWORK_BURST:
                    num_particles = 100;
                    for (int i = 0; i < num_particles; i++) {
                        if (fw->particle_count < MAX_PARTICLES) {
                            Particle *p = &fw->particles[fw->particle_count++];
                            p->active = true;
                            p->position = fw->position;
                            float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                            float speed = (float)GetRandomValue(1, 6);
                            p->velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
                            p->color = fw->color;
                            p->lifespan = (float)GetRandomValue(80, 130) / 100.0f;
                            p->is_secondary = false;
                        }
                    }
                    break;
                case FIREWORK_CRACKLE_ROCKET:
                    num_particles = 50;
                    for (int i = 0; i < num_particles; i++) {
                        if (fw->particle_count < MAX_PARTICLES) {
                            Particle *p = &fw->particles[fw->particle_count++];
                            p->active = true;
                            p->position = fw->position;
                            float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                            float speed = (float)GetRandomValue(2, 7);
                            p->velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
                            p->color = fw->color;
                            p->lifespan = (float)GetRandomValue(100, 150) / 100.0f;
                            p->is_secondary = false;
                        }
                    }
                    break;
                default: break;
            }
        }
    } else { // Is exploded
        if (fw->type == FIREWORK_FOUNTAIN && GetRandomValue(0, 1) == 0) {
            int fountain_particles = 5;
            for (int i = 0; i < fountain_particles; i++) {
                if (fw->particle_count < MAX_PARTICLES - fountain_particles) {
                    Particle *p = &fw->particles[fw->particle_count++];
                    p->active = true;
                    p->position = fw->position;
                    p->velocity = (Vector2){ (float)GetRandomValue(-15, 15) / 10.0f, (float)GetRandomValue(-10, -6) };
                    p->color = fw->color;
                    p->lifespan = (float)GetRandomValue(100, 150) / 100.0f;
                    p->is_secondary = false;
                }
            }
        }

        bool all_particles_dead = true;
        for (int i = 0; i < fw->particle_count; i++) {
            if (fw->particles[i].active) {
                all_particles_dead = false;
                fw->particles[i].position.x += fw->particles[i].velocity.x * dt * 60;
                fw->particles[i].position.y += fw->particles[i].velocity.y * dt * 60;
                fw->particles[i].velocity.y += 0.15f * dt * 60;
                fw->particles[i].lifespan -= dt;

                if (fw->type == FIREWORK_CRACKLE_ROCKET && !fw->particles[i].is_secondary && fw->particles[i].lifespan < 0.5f) {
                    if (GetRandomValue(0, 15) == 0) {
                        for (int j = 0; j < 5; j++) {
                            if (fw->particle_count < MAX_PARTICLES) {
                                Particle *sp = &fw->particles[fw->particle_count++];
                                sp->active = true;
                                sp->position = fw->particles[i].position;
                                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                                float speed = (float)GetRandomValue(1, 3);
                                sp->velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
                                sp->color = GOLD;
                                sp->lifespan = 0.5f;
                                sp->is_secondary = true;
                            }
                        }
                        fw->particles[i].lifespan = 0; // Kill parent particle
                    }
                }

                fw->particles[i].color.a = (unsigned char)(fmax(0.0, fw->particles[i].lifespan * 255.0));
                if (fw->particles[i].lifespan <= 0) {
                    fw->particles[i].active = false;
                }
            }
        }

        if (all_particles_dead) {
            InitFirework(fw, index);
        }
    }
}

void UpdateAndDrawTextSparkles(Particle sparkles[], int max_sparkles, AnimatedLetter letters[], int message_len, float dt) {
    // Update and draw existing sparkles
    for (int i = 0; i < max_sparkles; i++) {
        if (sparkles[i].active) {
            sparkles[i].position.x += sparkles[i].velocity.x * dt * 60;
            sparkles[i].position.y += sparkles[i].velocity.y * dt * 60;
            sparkles[i].lifespan -= dt;
            sparkles[i].color.a = (unsigned char)(fmax(0.0, sparkles[i].lifespan * 255.0));

            if (sparkles[i].lifespan <= 0) {
                sparkles[i].active = false;
            }

            DrawCircleV(sparkles[i].position, sparkles[i].lifespan * 3.0f, sparkles[i].color);
        }
    }

    // Spawn new sparkles
    if (GetRandomValue(0, 5) == 0) {
        int letter_index = GetRandomValue(0, message_len - 1);
        if (letters[letter_index].active && fabsf(letters[letter_index].velocity.y) < 0.1f) {
            for (int i = 0; i < max_sparkles; i++) {
                if (!sparkles[i].active) {
                    sparkles[i].active = true;
                    float char_width = MeasureText(letters[letter_index].character, 80);
                    sparkles[i].position = (Vector2){
                        letters[letter_index].position.x + GetRandomValue(0, char_width),
                        letters[letter_index].position.y + GetRandomValue(-40, 10)
                    };
                    sparkles[i].velocity = (Vector2){ (float)GetRandomValue(-5, 5) / 10.0f, (float)GetRandomValue(-5, 5) / 10.0f };
                    sparkles[i].color = (Color){ 255, 255, GetRandomValue(150, 255), 255 };
                    sparkles[i].lifespan = (float)GetRandomValue(2, 6) / 10.0f;
                    break; // Spawn one at a time
                }
            }
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Happy Diwali!");

    Diya diyas[NUM_DIYAS];
    for(int i = 0; i < NUM_DIYAS; i++) {
        diyas[i].position = (Vector2){ 150.0f + i * 250.0f, 550.0f };
    }

    Firework fireworks[MAX_FIREWORKS];
    for (int i = 0; i < MAX_FIREWORKS; i++) {
        InitFirework(&fireworks[i], i);
    }

    Particle text_sparkles[MAX_TEXT_SPARKLES] = {0};
    Sparkler sparkler = { 0 };

    const char* message = "Happy Diwali!";
    int message_len = strlen(message);
    AnimatedLetter letters[message_len];
    int text_size = 80;
    float letter_spacing = 10.0f;
    float total_text_width = 0;

    for (int i = 0; i < message_len; i++) {
        char single_char_str[2] = { message[i], '\0' };
        total_text_width += MeasureText(single_char_str, text_size) + letter_spacing;
    }

    float current_x = (SCREEN_WIDTH - total_text_width) / 2.0f;

    for (int i = 0; i < message_len; i++) {
        letters[i].character[0] = message[i];
        letters[i].character[1] = '\0';
        float char_width = MeasureText(letters[i].character, text_size);
        letters[i].final_y = (float)SCREEN_HEIGHT / 2.5f;
        letters[i].position = (Vector2){ current_x, -50.0f };
        current_x += char_width + letter_spacing;

        letters[i].velocity = (Vector2){ 0, 0 };
        letters[i].alpha = 0.0f;
        letters[i].rotation = (float)GetRandomValue(-20, 20);
        letters[i].color = WHITE;
        letters[i].active = false;
        letters[i].brightness = 1.0f;
    }

    float letter_timer = 0.0f;
    int current_letter = 0;
    float sparkle_run_timer = 0.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (int i = 0; i < MAX_FIREWORKS; i++) {
            UpdateFirework(&fireworks[i], dt, i);
        }

        bool all_letters_settled = current_letter >= message_len;
        if (!all_letters_settled) {
            letter_timer += dt;
            if (letter_timer > 0.08f && current_letter < message_len) {
                letters[current_letter].active = true;
                current_letter++;
                letter_timer = 0.0f;
            }
        }

        for (int i = 0; i < message_len; i++) {
            if (letters[i].active) {
                if (letters[i].alpha < 1.0f) letters[i].alpha += dt * 4.0f;
                letters[i].color = ColorAlpha(YELLOW, letters[i].alpha);
                
                float distance = letters[i].final_y - letters[i].position.y;
                float speed = distance * 0.1f;

                letters[i].position.y += speed * dt * 60;
                letters[i].rotation *= 0.98f; // Gentler rotation fade

                letters[i].brightness -= 0.5 * dt;
                if (letters[i].brightness < 1.0f) letters[i].brightness = 1.0f;
                letters[i].color.r = (unsigned char)(255 * letters[i].brightness);
                letters[i].color.g = (unsigned char)(255 * letters[i].brightness);
                letters[i].color.b = (unsigned char)(200 * letters[i].brightness);
            }
        }

        if (all_letters_settled) {
            sparkle_run_timer += dt;
            if (!sparkler.active && sparkle_run_timer > 2.0f) {
                sparkler.active = true;
                sparkler.target_letter = 0;
                sparkler.position = (Vector2){ letters[0].position.x, letters[0].final_y };
                sparkle_run_timer = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < NUM_DIYAS; i++) {
            DrawDiya(diyas[i], i);
        }

        for (int i = 0; i < MAX_FIREWORKS; i++) {
            if (!fireworks[i].exploded) {
                DrawCircleV(fireworks[i].position, 3, fireworks[i].color);
            }
            for (int j = 0; j < fireworks[i].particle_count; j++) {
                if (fireworks[i].particles[j].active) {
                    DrawCircleV(fireworks[i].particles[j].position, 2, fireworks[i].particles[j].color);
                }
            }
        }

        for (int i = 0; i < message_len; i++) {
            if (letters[i].active) {
                DrawTextPro(GetFontDefault(), letters[i].character, (Vector2){letters[i].position.x, letters[i].position.y}, (Vector2){0,0}, letters[i].rotation, text_size, 1, letters[i].color);
            }
        }

        if (current_letter >= message_len) {
            UpdateAndDrawTextSparkles(text_sparkles, MAX_TEXT_SPARKLES, letters, message_len, dt);
        }

        if (sparkler.active) {
            Vector2 target_pos = { letters[sparkler.target_letter].position.x, letters[sparkler.target_letter].final_y };
            float dist_x = target_pos.x - sparkler.position.x;
            float dist_y = target_pos.y - sparkler.position.y;
            sparkler.position.x += dist_x * 0.1f;
            sparkler.position.y += dist_y * 0.1f;

            if (fabs(dist_x) < 5.0f) {
                letters[sparkler.target_letter].brightness = 2.0f;
                if (sparkler.target_letter < message_len - 1) {
                    sparkler.target_letter++;
                } else {
                    sparkler.active = false;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
