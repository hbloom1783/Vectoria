#include "Font.h"

#include "vector"
using std::vector;

namespace Models
{
	Font::Font(const XmlNode& fontNode)
	{
		this->LoadXml(fontNode);
	}

	void Font::LoadXml(const XmlNode& fontNode)
	{
		this->size = Vector2(
			fontNode.GetAttribute("sizex").AsFloat(),
			fontNode.GetAttribute("sizey").AsFloat());

		Model baseModel = Model(fontNode).GetPrimitive("basemodel").asSubmodel;

		vector<XmlNode> letters = fontNode.GetChildren("letter");
		for (auto letter = letters.begin(); letter != letters.end(); letter++)
		{
			Model letterModel = Model();
			vector<XmlNode> strokes = letter->GetChildren("stroke");
			for (auto stroke = strokes.begin(); stroke != strokes.end(); stroke++)
			{
				string strokeName = stroke->GetAttribute("name");
				Primitive strokePrim = baseModel.GetPrimitive(strokeName);
				switch (strokePrim.type)
				{
				case primLineSegment:
					letterModel.AddLineSegment(
						strokeName,
						strokePrim.asLine,
						strokePrim.color);
					break;
				case primTriangle:
					letterModel.AddTriangle(
						strokeName,
						strokePrim.asTri,
						strokePrim.color);
					break;
				case primSubmodel:
					letterModel.AddSubmodel(
						strokeName,
						strokePrim.asSubmodel);
					break;
				}
			}

			this->letterModels[letter->GetAttribute("char")[0]] = letterModel;
		}
	}

	Model Font::ModelText(string text)
	{
		Model textModel = Model();

		float textWidth = (2 + ((text.size() - 1) * 2.5)) * this->size.x;
		Vector2 leftOffset = Vector2(this->size.x - textWidth / 2, 0);
		Vector2 kernVector = Vector2(this->size.x * 2.5);

		for (int idx = 0; idx < text.size(); idx++)
		{
			if (this->letterModels.find(text[idx]) != this->letterModels.end())
			{
				Vector2 kern = leftOffset + (idx * kernVector);

				char letterName[] = "letter_000";
				sprintf(letterName, "letter_%03d", idx);

				textModel.AddSubmodel(
					letterName,
					this->letterModels[text[idx]].OffsetModel(kern));
			}
		}

		//textModel.offset = Vector2();

		return textModel;
	}
}