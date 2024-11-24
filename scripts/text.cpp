#define _TEXT
#include "text.h"

static std::map<std::string, std::map<char, cChar>> programFonts;

static constexpr uint fChar = 32;
static constexpr uint lChar = 127;

static constexpr uint charSizeExtraSmall = 16;
static constexpr uint charSizeSmall = 32;
static constexpr uint charSizeNormal = 64;
static constexpr uint charSizeBig = 128;
static constexpr uint charSizeLarge = 256;
static constexpr uint charSizeHuge = 512;

static uint FBO = 0;
static uint VAO, VBO, EBO;

Shader shader;

extern const float* _identityMatrix;

/// <summary>
/// Load a font into a map with some size
/// </summary>
/// <param name="map">: The map that fonts will load into</param>
/// <param name="face">: The FT_Face that contains the font you want to use</param>
/// <param name="size">: The pixel size of the font</param>
/// <returns>True/False on if it is able to load all the characters</returns>
bool loadSizeMap(std::map<char, cChar> &map, FT_Face& face, uint size) {
	//Set to whatever size
	FT_Set_Pixel_Sizes(face, 0, size);

	for (char c = fChar; c < lChar; c++) {

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			return false;

		uint charTexture = 0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &charTexture);
		glBindTexture(GL_TEXTURE_2D, charTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 0, 0, 0, 0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		map.insert({ c, cChar(charTexture, {(float)face->glyph->bitmap_left / size, (float)face->glyph->bitmap_top / size}, {(float)face->glyph->bitmap.width / size, (float)face->glyph->bitmap.rows / size}, (float)(face->glyph->advance.x >> 6) / size) });
	}
	return true;
}


/// <summary>
/// Load a font with the default sizes into a global fonts area
/// </summary>
/// <param name="font">: The font to be loaded</param>
/// <param name="name">: The name that you will use to access the font</param>
/// <returns>True/False on if the font is able to be loaded</returns>
bool loadFont(std::string font, std::string name) {
	programFonts.emplace(font, std::map<char, cChar>());

	FT_Library fontLib;
	FT_Face fontFace;
	if (FT_Init_FreeType(&fontLib))
		return false;
	if (FT_New_Face(fontLib, font.c_str(), 0, &fontFace))
		return false;

	std::map<char, cChar>* map = nullptr;
	map = &programFonts[name + "_EXSMALL"];
	loadSizeMap(*map, fontFace, charSizeExtraSmall);
	map = &programFonts[name + "_SMALL"];
	loadSizeMap(*map, fontFace, charSizeSmall);
	map = &programFonts[name + "_NORMAL"];
	loadSizeMap(*map, fontFace, charSizeNormal);
	map = &programFonts[name + "_BIG"];
	loadSizeMap(*map, fontFace, charSizeBig);
	map = &programFonts[name + "_LARGE"];
	loadSizeMap(*map, fontFace, charSizeLarge);
	map = &programFonts[name + "_HUGE"];
	loadSizeMap(*map, fontFace, charSizeHuge);
	
	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLib);

	return true;
}


/// <summary>
/// Get the data of some fonts character
/// </summary>
/// <param name="name">: Name of the font</param>
/// <param name="character">: The character you want</param>
/// <returns>A struct that contains all the information for the character and texture</returns>
cChar getChar(std::string name, char character) {
	return programFonts[name][character];
}


/// <summary>
/// Draw text from some font to screen with some size and starting position
/// </summary>
/// <param name="font">: The font to be used</param>
/// <param name="text">: Text that will be drawn</param>
/// <param name="fontSize">: Font size</param>
/// <param name="x">: Starting 'x' position</param>
/// <param name="yT">: Starting 'y' position</param>
void renderTextBuffer(std::string font, std::string text, float fontSize, float x, float yT) {
	cChar curChar;
	float w = 0;
	float h = 0;

	for (uint i = 0; i < text.length(); i++) {
		char c = text[i];
		curChar = getChar(font, c);
		if (c == ' ') {
			x += curChar.advance * fontSize;
			continue;
		}

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, curChar.texTarget);

		x += curChar.bearing.x * fontSize;

		float y = yT + (curChar.bearing.y - curChar.size.y) * fontSize;
		w = curChar.size.x * fontSize;
		h = curChar.size.y * fontSize;

		float vertices[20] = {
			x,		y + h,	0,			0, 0,
			x + w,	y + h,	0,			1, 0,
			x + w,	y,		0,			1, 1,
			x,		y,		0,			0, 1
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		x += curChar.advance * fontSize;
	}
}


/// <summary>
/// Creates a text texture to be used by some object
/// <para>Supports line breaks</para>
/// </summary>
/// <param name="texture">: The output texture</param>
/// <param name="fontSize">: A relative font size to the object</param>
/// <param name="lineSize">: A relative line size to the object</param>
/// <param name="size">: Object size</param>
/// <param name="yStart">: Starting y position</param>
/// <param name="margin">: Text margin</param>
/// <param name="font">: Font you want</param>
/// <param name="text">: Text that will be shown on texture</param>
/// <returns>Text drawn to 'texture'</returns>
void createTextTexture(uint& texture, float fontSize, float lineSize, Vec2 size, float yStart, float margin, uint mode, std::string font, std::string text) {
	uint screenX = size.x * _Height;
	uint screenY = size.y * _Height;
	cChar curChar;
	float ratio = size.x / size.y;

    if (text == "" || font == "" || fontSize == 0 || size.x == 0 || size.y == 0 || mode > 2) {
        return;
    }

	//Draw text centered both vertically and horizontally
	//Does a recurse function move to make things easier
	if (mode == TEXT_CENTER_RENDER) {
		Vec2 Pos = { -ratio + margin, yStart - lineSize };
		for (int i = 0; i < text.length(); i++) {
			char c = text[i];
			if (c == '\n')
			{
				Pos.x = -ratio + margin;
				Pos.y -= lineSize;
				continue;
			}

			curChar = programFonts[font][c];

			Pos.x += (curChar.advance + curChar.bearing.x) * fontSize;
			if (Pos.x > ratio + margin)
			{
				Pos.x = -ratio + margin;
				Pos.y -= lineSize;
			}
		}
		createTextTexture(texture, fontSize, lineSize, size, (yStart - Pos.y) / 2 + (fontSize / 2), margin, TEXT_LINE_CENTER_RENDER, font, text);
		return;
	}



	glm::mat4 ortho = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);

	shader.active();
	shader.setInt("texTarget", 0);
	shader.setMat4("projection", ortho);
	shader.setMat4("view", _identityMatrix);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenX, screenY, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glViewport(0, 0, screenX, screenY);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);


	//Draws text on the left hand side of the text
	if (mode == TEXT_LINE_LEFT_RENDER)
	{
		Vec2 realPos = { -ratio + margin, yStart - lineSize };
		Vec2 fakePos = realPos;
		bool oneWord = true;
		std::string word;
		for (uint i = 0; i < text.length(); i++) {
			char c = text[i];

			if (c == '\n') {
				if (word.length() != 0) {
					renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
					word.clear();
				}

				fakePos.x = -ratio + margin;
				fakePos.y -= lineSize;
				realPos = fakePos;
			}

			curChar = getChar(font, c);

			if (c == ' ') {
				if (word.length() != 0)
				{
					renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
					word.clear();
				}
				realPos.x = fakePos.x + (curChar.bearing.x + curChar.advance) * fontSize;
				oneWord = false;
			}

			fakePos.x += (curChar.bearing.x + curChar.advance) * fontSize;
			if (c != ' ')
				word += c;

			if (fakePos.x > ratio + margin) {
				if (oneWord)
				{
					if (word.length() == 1) {
						renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
						word.clear();
						fakePos.x = -ratio + margin;
						fakePos.y -= lineSize;
						realPos = fakePos;
						continue;
					}
					if (word.length() != 0) {
						word.pop_back();
						renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
						word.clear();
					}
					fakePos.x = -ratio;
					fakePos.y -= lineSize;
					realPos = fakePos;
					i--;
					continue;
				}
				if (word.length() != 0) {
					i -= word.length();
					word.clear();
				}
				fakePos.x = -ratio;
				fakePos.y -= lineSize;
				realPos = fakePos;
				oneWord = true;
				continue;
			}

			if (i == text.length() - 1)
				renderTextBuffer(font, word, fontSize, realPos.x, realPos.y);
		}
	}

	//Draws text centered within the texture
	//Best with text that includes line breaks often since it will act the same as left drawing if the line fills
	if (mode == TEXT_LINE_CENTER_RENDER)
	{
		float wordWidth = 0;
		Vec2 pos = { 0, yStart - fontSize };
		std::string sentence;
		std::string word;
		bool oneWord = true;
		for (uint i = 0; i < text.length(); i++) {
			char c = text[i];

			if (c == '\n')
			{
				renderTextBuffer(font, sentence, fontSize, -pos.x / 2, pos.y);
				sentence.clear();
				word.clear();
				pos.x = 0;
				pos.y -= lineSize;
				wordWidth = 0;
				oneWord = true;
			}

			curChar = getChar(font, c);

			float charWidth = (curChar.bearing.x + curChar.advance) * fontSize;

			pos.x += charWidth;
			wordWidth += charWidth;

			sentence += c;
			if (c != ' ')
				word += c;
			else {
				word.clear();
				wordWidth = 0;
				oneWord = false;
			}
			
			if (pos.x > 2 * (ratio + margin)) {
				if (!oneWord) {
					sentence.erase(sentence.end() - word.length(), sentence.end());
					i -= word.length();
					renderTextBuffer(font, sentence, fontSize, -ratio + wordWidth / 2, pos.y);
					sentence.clear();
					pos.x = 0;
					pos.y -= lineSize;
					oneWord = true;
					continue;
				}
				else
				{	
					if (sentence.length() == 1) {
						renderTextBuffer(font, sentence, fontSize, -ratio + charWidth / 2, pos.y);
						sentence.clear();
						word.clear();
						wordWidth = 0;
						pos.x = 0;
						pos.y -= lineSize;
						continue;
					}
					renderTextBuffer(font, sentence, fontSize, -ratio + charWidth / 2, pos.y);
					i--;
				}
				sentence.clear();
				word.clear();
				wordWidth = 0;
				pos.x = 0;
				pos.y -= lineSize;

				oneWord = true;
			}
			

			if (i == text.length() - 1)
				renderTextBuffer(font, sentence, fontSize, -pos.x / 2, pos.y);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _Width, _Height);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}


namespace Text {
	void start() {
        if (!loadFont("./fonts/CascadiaCode.ttf", "CascadiaCode")) {
            std::cout << "Failed to load font\n";
        }
        if (!loadFont("./fonts/CascadiaCode-Light.ttf", "CascadiaCodeLight")) {
            std::cout << "Failed to load font\n";
        }

		glGenFramebuffers(1, &FBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STREAM_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STREAM_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		addShader("textShader", "./shaders/TextVertexShader.vert", "./shaders/TextFragmentShader.frag");
		shader.use(getShader("textShader"));
	}

	void end() {
		for (auto& map : programFonts) {
			for (int c = fChar; c < lChar; c++) {
				glDeleteTextures(1, &(map.second)[c].texTarget);
				map.second.clear();
			}
		}
		programFonts.clear();
	}

}