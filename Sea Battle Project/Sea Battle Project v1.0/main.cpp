#define OLC_PGE_APPLICATION
#include <typeinfo>
#include <iostream>
#include "olcPixelGameEngine.h" //890 строчка - функции для рисования

#define FULLSCREEN 0

struct Obj_t
{
    //DrawPartialRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale, const olc::Pixel& tint)
    std::shared_ptr<olc::Decal> img;
    olc::vf2d pos; // точка, в которую будет помещаться наш кусок декала на экране
    olc::vf2d source_pos; // левая верхняя точка куска декала  i.e.: (0, 100)
    olc::vf2d source_size; // размеры куска декала декала i.e.: (500, 19)
    // =\ получаем кусок декала с началом (левый верхний угол) в (0, 100) от оригинала
    // =/ и правым нижним углом в (500, 119)
    olc::vf2d scale = { 1.0f, 1.0f };
    float fAngle = 0.0f; // угол вращения
    olc::vf2d center; // центр, относительно которого будет вращаться декал
};

struct Str_t
{
    //DrawStringDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
    olc::vf2d pos;
    std::string sText;
    olc::Pixel col = olc::WHITE;
    olc::vf2d scale;
};

class MyApp : public olc::PixelGameEngine {
private:
    //private class variables

    unsigned int m_score = 0;
    float m_SpeedAmplifier = 120.0f; // old: 80.0f
    bool flag_torpeda = 0;

    olc::vf2d m_BoatPos = olc::vf2d((ScreenWidth() / 2) + 316, 159); // olc::vf2d(ScreenWidth() - (ScreenWidth() * 4 / 5), ScreenHeight() - (ScreenHeight() * 4 / 5));
    olc::vf2d m_TorpPos = olc::vf2d(625, 675);

    olc::vf2d m_TorpVel = olc::vf2d(0, -1);
    olc::vf2d m_BoatVel = olc::vf2d(1, 0);

    olc::vf2d m_CamPos = olc::vf2d(-1275 / 2, -675 / 2);
    olc::vf2d m_CamVel = olc::vf2d(0, 0);

    Str_t str_score;
    Obj_t dyn_boat_1;
    Obj_t dyn_boat_2; 
    Obj_t dyn_torpeda;
    Obj_t bg_uwater;
    Obj_t bg_dwater;
    Obj_t hg_prihole;

public:
    //public class variables

    //public class functions

    void CheckForCrossings() {
        /*
        if (m_BoatPos.x <= 0) {
            m_BoatVel.x = 1;
            dyn_boat_1.source_pos = olc::vf2d(475, 0);
            dyn_boat_1.source_size = olc::vf2d(473, 395); // 948 - 475 = 473 :c
        }
        if (m_BoatPos.x + (dyn_boat_1.source_size.x * dyn_boat_1.scale.x) >= ScreenWidth()) {
            m_BoatVel.x = -1;
            dyn_boat_1.source_pos = olc::vf2d(0, 0);
            dyn_boat_1.source_size = olc::vf2d(475, 395); // смотрит влево
        }
        
        if (m_BoatPos.x + (dyn_boat_1.source_size.x * dyn_boat_1.scale.x) <= 0) {
            m_BoatPos.x = ScreenWidth() + 150;
        }
        if (m_BoatPos.x >= ScreenWidth() + (dyn_boat_1.source_size.x * dyn_boat_1.scale.x)) {
            m_BoatPos.x = 0 - (dyn_boat_1.source_size.x * dyn_boat_1.scale.x);
        }
        */

        // Boat crosses the screen borders

        if (m_BoatPos.x > ScreenWidth()) {
            m_BoatPos.x = dyn_boat_1.source_size.x * -1 * dyn_boat_1.scale.x;
        }
        if (m_BoatPos.x + dyn_boat_1.source_size.x * dyn_boat_1.scale.x < 0) {
            m_BoatPos.x = ScreenWidth();
        }

        // Torpeda hits the boat

        if (flag_torpeda && m_TorpPos.y <= (m_BoatPos.y + 170) && m_TorpPos.x >= m_BoatPos.x && m_TorpPos.x <= (m_BoatPos.x + dyn_boat_1.source_size.x * dyn_boat_1.scale.x)){// - dyn_boat_1.source_pos.x)) {
            flag_torpeda = 0;
            //Printinfo();
            m_TorpPos = olc::vf2d((ScreenWidth() / 2) - 15.5f, 675);
            //std::cout << "caboom!\n";
            m_BoatVel.x *= -1;
            if (m_BoatVel.x == 1) {
                dyn_boat_1.source_pos = olc::vf2d(475, 0);
                dyn_boat_1.source_size = olc::vf2d(473, 395);
            }
            else if (m_BoatVel.x == -1) {
                dyn_boat_1.source_pos = olc::vf2d(0, 0);
                dyn_boat_1.source_size = olc::vf2d(475, 395);
            }
            m_score++;
            str_score.sText = std::to_string(m_score);
        }

        else if (flag_torpeda && m_TorpPos.y <= (m_BoatPos.y + 125) && (m_TorpPos.x < m_BoatPos.x || m_TorpPos.x > m_BoatPos.x + dyn_boat_1.source_size.x * dyn_boat_1.scale.x)){// - dyn_boat_1.source_pos.x))) {
            //Printnuinfo();
            //std::cout << "miss\n";
            flag_torpeda = 0;
            m_TorpPos = olc::vf2d((ScreenWidth() / 2) - 15.5f, 675);
        }
    }
    
    void Printinfo() {
        std::cout << "Torp pos: " << m_TorpPos << std::endl;
        std::cout << "Boat pos: " << m_BoatPos << std::endl;
        std::cout << "m_BoatPos.x + dyn_boat_1.source_size.x  * dyn_boat_1.scale.x: " << m_BoatPos.x + dyn_boat_1.source_size.x * dyn_boat_1.scale.x << std::endl;// -dyn_boat_1.source_pos.x << std::endl;
    }

    void MoveTorpeda(float fElapsedTime) {
        CheckForCrossings();
        m_TorpPos += m_TorpVel * 250 * fElapsedTime;
        dyn_torpeda.pos = m_TorpPos;
    }
    
    void MoveBoats(float fElapsedTime) {
        CheckForCrossings();
        m_BoatPos += m_BoatVel * m_SpeedAmplifier * fElapsedTime;
        dyn_boat_1.pos = m_BoatPos;
        m_SpeedAmplifier += 1.0f * fElapsedTime;
    }

    void MoveCamera(float fElapsedTime) {
        m_CamPos += m_CamVel * m_SpeedAmplifier * fElapsedTime;
        hg_prihole.pos = m_CamPos;
    }

    void UpdateCamKeys(float fElapsedTime) {

        // Camera Movement (CAUTION: EPILEPSY THREAT)

        if ((GetKey(olc::Key::UP).bHeld) && (GetKey(olc::Key::DOWN).bHeld) && (GetKey(olc::Key::LEFT).bHeld)) {
            m_CamVel = olc::vf2d(-1, 0); // U + D ++ L
        }
        else if ((GetKey(olc::Key::UP).bHeld) && (GetKey(olc::Key::DOWN).bHeld) && (GetKey(olc::Key::RIGHT).bHeld)) {
            m_CamVel = olc::vf2d(1, 0); // U + D ++ R
        }
        else if ((GetKey(olc::Key::LEFT).bHeld) && (GetKey(olc::Key::RIGHT).bHeld) && (GetKey(olc::Key::DOWN).bHeld)) {
            m_CamVel = olc::vf2d(0, 1); // L + R ++ D
        }
        else if ((GetKey(olc::Key::LEFT).bHeld) && (GetKey(olc::Key::RIGHT).bHeld) && (GetKey(olc::Key::UP).bHeld)) {
            m_CamVel = olc::vf2d(0, -1); // L + R ++ U
        }
        else if (((GetKey(olc::Key::UP).bHeld) && (GetKey(olc::Key::DOWN).bHeld)) || ((GetKey(olc::Key::LEFT).bHeld) && (GetKey(olc::Key::RIGHT).bHeld))) {
            m_CamVel = olc::vf2d(0, 0); // overloading keys?
        }
        else if ((GetKey(olc::Key::UP).bHeld) && (GetKey(olc::Key::RIGHT).bHeld)) {
            m_CamVel = olc::vf2d(1, -1); // U ++ R
        }
        else if ((GetKey(olc::Key::UP).bHeld) && (GetKey(olc::Key::LEFT).bHeld)) {
            m_CamVel = olc::vf2d(-1, -1); // U ++ L
        }
        else if ((GetKey(olc::Key::DOWN).bHeld) && (GetKey(olc::Key::RIGHT).bHeld)) {
            m_CamVel = olc::vf2d(1, 1); // D ++ R
        }
        else if ((GetKey(olc::Key::DOWN).bHeld) && (GetKey(olc::Key::LEFT).bHeld)) {
            m_CamVel = olc::vf2d(-1, 1); // D ++ L
        }
        else if (GetKey(olc::Key::RIGHT).bHeld) {
            m_CamVel = olc::vf2d(1, 0); // R
        }
        else if (GetKey(olc::Key::LEFT).bHeld) {
            m_CamVel = olc::vf2d(-1, 0); // L
        }
        else if (GetKey(olc::Key::UP).bHeld) {
            m_CamVel = olc::vf2d(0, -1); // U
        }
        else if (GetKey(olc::Key::DOWN).bHeld) {
            m_CamVel = olc::vf2d(0, 1); // D
        }

        if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::RIGHT).bHeld) {
            MoveCamera(fElapsedTime); // if something is pressed, change the CamPos
        }
    }

    void UpdateKeys(float fElapsedTime) {
        if (GetKey(olc::Key::ESCAPE).bPressed) {
            olc_Terminate();
        }
        if (GetKey(olc::Key::SPACE).bPressed && !(flag_torpeda)) {
            flag_torpeda = 1;
        }
        UpdateCamKeys(fElapsedTime);

        /*
        if (GetKey(olc::Key::Q).bHeld) {
            dyn_boat_1.fAngle += float(3.14f * fElapsedTime * float(m_SpeedAmplifier) / 180.0);
        }
        if (GetKey(olc::Key::W).bHeld) {
            dyn_boat_1.center.x += 5;
        }
        if (GetKey(olc::Key::S).bHeld) {
            dyn_boat_1.center.x -= 5;
        }
        if (GetKey(olc::Key::E).bHeld) {
            dyn_boat_1.center.y += 5;
        }
        if (GetKey(olc::Key::D).bHeld) {
            dyn_boat_1.center.y -= 5;
        }
        */
        /*
        if (GetKey(olc::Key::A).bHeld) {
            DrawStatic(bg_dwater);
        }
        if (GetKey(olc::Key::Q).bHeld) {
            DrawStatic(bg_uwater);
        }
        */
        
    }

    void DrawStatic(Obj_t& s) {
        /*
        DrawPartialRotatedDecal(
        const olc::vf2d& pos,
        olc::Decal* decal,
        const float fAngle,
        const olc::vf2d& center,
        const olc::vf2d& source_pos,
        const olc::vf2d& source_size,
        const olc::vf2d& scale,
        const olc::Pixel& tint
        )
        */
        DrawPartialRotatedDecal(
            s.pos,
            s.img.get(),
            s.fAngle,
            s.center,
            s.source_pos,
            s.source_size,
            s.scale, olc::WHITE);
    }

    void DrawRotStatic(Obj_t& s) {
        DrawRotatedDecal(
            s.pos,//+s.center,
            s.img.get(),
            s.fAngle,
            s.center,
            s.scale, olc::WHITE);

    }

    void DrawStringStatic(Str_t& s) {
        DrawStringDecal(
            s.pos,
            s.sText,
            s.col,
            s.scale);
    }

    //main overrided functions

    bool OnUserCreate() override
    {
        SetDecalMode(olc::DecalMode::ILLUMINATE);
        auto tempDynImg1 = new olc::Sprite("textures/pboatpartial.png"); // partial boat img
        auto tempDynImg2 = new olc::Sprite("textures/torpeda.png"); // partial torpeda img
        auto tempDynImg3 = new olc::Sprite("textures/prihole.png"); // eye-hole img
        auto tempBgImg = new olc::Sprite("textures/water.png"); // full water img

        hg_prihole = {
            std::make_shared<olc::Decal>(tempDynImg3),
            {-1275/2, -675/2}, // otrisovka
            {0, 0}, // part
            {2550, 1350}, // size
            {1.0f, 1.0f}, // scale
            0.0f, // angle
            {0, 0} // center
        };

        bg_uwater = { // from (0, 0) 1275 x 329
            std::make_shared<olc::Decal>(tempBgImg),
            {0, 0},
            {0, 0},
            {1275, 329},
            {1.0f, 1.0f},
            0.0f,
            {0, 0} // ((olc::vf2d(1275, 329)) / 2) // just (bg_uwater.source_size)/2, but bg_uwater.source_size haven't been initialized :c
        };

        bg_dwater = { // from (0, 329) 1275 x 675
            std::make_shared<olc::Decal>(tempBgImg),
            {0, 329},
            {0, 329},
            {1275, 346}, // 675 - 329 = 346 :c
            {1.0f, 1.0f},
            0.0f,
            {0, 0} // ((olc::vf2d(1275, 346)) / 2)
        };

        dyn_boat_1 = {
            std::make_shared<olc::Decal>(tempDynImg1), //475 - х между кораблями
            m_BoatPos,
            {475, 0}, // {0, 0}
            {473, 395}, // {475, 395} // смотрит влево
            { 0.45f, 0.45f }, // { 1.0f, 1.0f }
            0.0f,
            {0, 0}
        };

        dyn_torpeda = {
            std::make_shared<olc::Decal>(tempDynImg2),
            m_TorpPos,
            {343, 0}, // s_pos
            {30, 338}, // s_size
            { 0.54f, 0.25f},
            0.0f,
            {0, 0}
            // s_pos{343, 0} s_size{30, 338} - смотрит вверх;
            // s_pos{382, 0} s_size{30, 338} - смотрит вниз;
            // s_pos{0, 0} s_size{338, 30} - смотрит влево;
            // s_pos{0, 39} s_size{335, 29} - смотрит вправо.
        };

        str_score = {
            {1175, 45},
            "0",
            olc::RED,
            {4.0f, 4.0f}
        };

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::Pixel(51, 212, 223));

        DrawRotStatic(bg_uwater);
        FillRectDecal(dyn_boat_1.pos, dyn_boat_1.source_size * dyn_boat_1.scale, olc::RED);
        DrawStatic(dyn_boat_1);
        DrawStatic(bg_dwater);
        if (flag_torpeda) {
            DrawStatic(dyn_torpeda);
            MoveTorpeda(fElapsedTime);
        }
        DrawStatic(hg_prihole);
        DrawStringStatic(str_score);

        UpdateKeys(fElapsedTime);
        MoveBoats(fElapsedTime);
        
        
        //FillCircle(GetMouseX(), GetMouseY(), 1, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

        return true;
    }
};


int main() {
    MyApp demo = MyApp();
    if (demo.Construct(1275, 675, 1, 1, FULLSCREEN)) // (demo.Construct(425, 225, 3, 3, FULLSCREEN))(demo.Construct(1275, 675, 1, 1, FULLSCREEN))
        demo.Start();
    return 0;

}