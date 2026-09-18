#include "Settings.hpp"
#include "Graphics.hpp"
#include "BlockClass.hpp"
#include "BoxClass.hpp"
#include "TextClass.hpp"
#include "PictureClass.hpp"
#include "BlurClass.hpp"

// Статические методы класса
void Settings::Open(HINSTANCE instance)
{
	// Если окно уже открыто - выводим его наверх
	if (Graphics::graphics)
	{
		SetForegroundWindow(Graphics::graphics->GetWindow());
		return;
	}

	// Создание графического окна настроек
	new Graphics{ instance, SETTINGS_WINDOW_SIZE, SETTINGS_WINDOW_MIN_SIZE, ColorRGBA::Create(34, 30, 28) };

	// Создаём интерфейс
	//Box temp;
	//Formula formula{};
	//int random;
	//int count = 1000;
	//float size = (float)SETTINGS_WINDOW_SIZE.width / count;
	//float sizeY = (float)SETTINGS_WINDOW_SIZE.height / count * 2;
	//srand(0);
	//for (int i = 0; i != count; i++)
	//{
	//	random = rand() % SETTINGS_WINDOW_SIZE.height;
	//	temp = BoxClass::Create(L"box", 0, ColorRGBA::Create((rand()% 191) + 64, (rand() % 191) + 64, (rand() % 191) + 64, 1));
	//	formula.SetSides(SideFormula::CreateStatic(i * size), SideFormula::CreateStatic(static_cast<float>(random)), SideFormula::CreateStatic((i + 1) * size), SideFormula::CreateStatic(sizeY + static_cast<float>(random)));
	//	temp->SetFormula(formula);

	//	random = rand() % SETTINGS_WINDOW_SIZE.height;
	//	formula.SetSides(SideFormula::CreateStatic(i * size), SideFormula::CreateGlue(sizeY + static_cast<float>(random)), SideFormula::CreateStatic((i + 1) * size), SideFormula::CreateGlue(static_cast<float>(random)));
	//	temp->SetFormula(formula, AnimationProperties{ 500, 4000, AnimationCurve::EASE_IN_OUT_QUARTIC });;
	//}	

    Formula formula{};

    // Заголовок
    Text title = TextClass::Create(
        L"Title",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"Тест базовых блоков",
        34,
        DWRITE_FONT_WEIGHT_BOLD,
        DWRITE_TEXT_ALIGNMENT_LEADING
    );
    formula.SetSides(
        SideFormula::CreateStatic(40),
        SideFormula::CreateStatic(30),
        SideFormula::CreateGlue(40),
        SideFormula::CreateStatic(80)
    );
    title->SetFormula(formula);

    // 1. Box — обычный прямоугольник без скругления
    Box box = BoxClass::Create(
        L"Box",
        nullptr,
        ColorRGBA::Create(61, 54, 49),
        ColorRGBA::Create(249, 204, 34),
        2,
        0
    );
    formula.SetSides(
        SideFormula::CreateStatic(40),
        SideFormula::CreateStatic(120),
        SideFormula::CreateStatic(300),
        SideFormula::CreateStatic(250)
    );
    box->SetFormula(formula);

    Text boxLabel = TextClass::Create(
        L"Box label",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"Box",
        22,
        DWRITE_FONT_WEIGHT_BOLD
    );
    formula.SetSides(
        SideFormula::CreateStatic(40),
        SideFormula::CreateStatic(260),
        SideFormula::CreateStatic(300),
        SideFormula::CreateStatic(295)
    );
    boxLabel->SetFormula(formula);

    // 2. RoundBox — тот же BoxClass, но с радиусом скругления
    Box roundBox = BoxClass::Create(
        L"RoundBox",
        nullptr,
        ColorRGBA::Create(61, 54, 49),
        ColorRGBA::Create(249, 120, 34),
        2,
        18
    );
    formula.SetSides(
        SideFormula::CreateStatic(340),
        SideFormula::CreateStatic(120),
        SideFormula::CreateStatic(600),
        SideFormula::CreateStatic(250)
    );
    roundBox->SetFormula(formula);

    Text roundBoxLabel = TextClass::Create(
        L"RoundBox label",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"RoundBox",
        22,
        DWRITE_FONT_WEIGHT_BOLD
    );
    formula.SetSides(
        SideFormula::CreateStatic(340),
        SideFormula::CreateStatic(260),
        SideFormula::CreateStatic(600),
        SideFormula::CreateStatic(295)
    );
    roundBoxLabel->SetFormula(formula);

    // 3. Text — самостоятельный текстовый блок
    Text textBlock = TextClass::Create(
        L"Text block",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"TextClass\nвывод многострочного текста",
        24,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_TEXT_ALIGNMENT_CENTER,
        true
    );
    formula.SetSides(
        SideFormula::CreateStatic(640),
        SideFormula::CreateStatic(120),
        SideFormula::CreateStatic(980),
        SideFormula::CreateStatic(250)
    );
    textBlock->SetFormula(formula);

    Text textLabel = TextClass::Create(
        L"Text label",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"Text",
        22,
        DWRITE_FONT_WEIGHT_BOLD
    );
    formula.SetSides(
        SideFormula::CreateStatic(640),
        SideFormula::CreateStatic(260),
        SideFormula::CreateStatic(980),
        SideFormula::CreateStatic(295)
    );
    textLabel->SetFormula(formula);

    // 4. Picture — вывод изображения
    Box pictureFrame = BoxClass::Create(
        L"Picture frame",
        nullptr,
        ColorRGBA::Create(61, 54, 49),
        ColorRGBA::Create(249, 204, 34),
        2,
        8
    );
    formula.SetSides(
        SideFormula::CreateStatic(40),
        SideFormula::CreateStatic(350),
        SideFormula::CreateStatic(300),
        SideFormula::CreateStatic(560)
    );
    pictureFrame->SetFormula(formula);

    Picture picture = PictureClass::Create(
        L"Picture",
        pictureFrame,
        L"GitHub.png"
    );
    formula.SetSides(
        SideFormula::CreateStatic(80),
        SideFormula::CreateStatic(55),
        SideFormula::CreateStatic(180),
        SideFormula::CreateStatic(155)
    );
    picture->SetFormula(formula);

    Text pictureLabel = TextClass::Create(
        L"Picture label",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"Picture",
        22,
        DWRITE_FONT_WEIGHT_BOLD
    );
    formula.SetSides(
        SideFormula::CreateStatic(40),
        SideFormula::CreateStatic(570),
        SideFormula::CreateStatic(300),
        SideFormula::CreateStatic(605)
    );
    pictureLabel->SetFormula(formula);

    // 5. Blur — размытие области под блоком
    Box blurBackground = BoxClass::Create(
        L"Blur background",
        nullptr,
        ColorRGBA::Create(248, 54, 16),
        ColorRGBA::Create(249, 204, 34),
        2,
        8
    );
    formula.SetSides(
        SideFormula::CreateStatic(340),
        SideFormula::CreateStatic(350),
        SideFormula::CreateStatic(600),
        SideFormula::CreateStatic(560)
    );
    blurBackground->SetFormula(formula);

    Picture picture2 = PictureClass::Create(
        L"Picture",
        blurBackground,
        L"GitHub.png"
    );
    formula.SetSides(
        SideFormula::CreateStatic(80),
        SideFormula::CreateStatic(55),
        SideFormula::CreateStatic(180),
        SideFormula::CreateStatic(155)
    );
    picture2->SetFormula(formula);

    Blur blur = BlurClass::Create(
        L"Blur",
        nullptr,
        10.f
    );
    formula.SetSides(
        SideFormula::CreateStatic(380),
        SideFormula::CreateStatic(390),
        SideFormula::CreateStatic(560),
        SideFormula::CreateStatic(520)
    );
    blur->SetFormula(formula);

    Text blurLabel = TextClass::Create(
        L"Blur label",
        nullptr,
        ColorRGBA::Create(255, 246, 240),
        L"Blur",
        22,
        DWRITE_FONT_WEIGHT_BOLD
    );
    formula.SetSides(
        SideFormula::CreateStatic(340),
        SideFormula::CreateStatic(570),
        SideFormula::CreateStatic(600),
        SideFormula::CreateStatic(605)
    );
    blurLabel->SetFormula(formula);

}
