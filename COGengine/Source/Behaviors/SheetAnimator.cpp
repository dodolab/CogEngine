
#include "SheetAnimator.h"
#include "CogEngine.h"
#include "Node.h"
#include "ResourceCache.h"
#include "EntityStorage.h"

namespace Cog {


	void SheetAnimator::Load(Setting& setting) {
		// load animation by name
		string animation = setting.GetItemVal("animation");
		auto resCache = GETCOMPONENT(ResourceCache);
		contextStack.SetRootNode(resCache->GetAnimation(animation));
	}

	void SheetAnimator::OnStart() {
		if (!contextStack.GetRootNode()) {
			CogLogError("SheetAnimator", "Animation cant' run, entity is null");
			Finish();
			return;
		}
	}

	void SheetAnimator::Update(const uint64 delta, const uint64 absolute) {

		// get next frame
		this->contextStack.MoveToNext(delta, CogEngine::GetInstance().GetFps());

		if (this->contextStack.Ended()) {
			Finish();
		}
		else {
			// get actual frame index
			int actualIndex = (int)contextStack.GetActualProgress();
			spt<SheetAnim> actualNode = static_pointer_cast<SheetAnim>(contextStack.GetContext().GetActualChild());

			if (actualNode->GetFrames() > 1 || actualNode->GetLines() > 1) {
				// image is a spritesheet (more than one sprite per image)
				string imagePath = actualNode->GetSheet(0);
				spt<ofImage> spriteSheet = CogGet2DImage(imagePath);

				// calculate image offset
				int frameIndex = actualIndex + actualNode->GetStart();

				if (owner->HasMeshType(MeshType::SPRITE)) {
					// render as a SpriteMesh (better performance than Image, because of using the SpriteSheetManager)

					// todo: recalculation always...
					auto spriteSet = owner->GetMesh<SpriteMesh>()->GetSpriteSet();
					owner->GetMesh<SpriteMesh>()->SetSprite(Sprite(spriteSet, frameIndex));
				}
				else {
					if (!owner->HasMeshType(MeshType::IMAGE)) {
						owner->SetMesh(spt<Image>(new Image(spriteSheet)));
					}

					// render as a ImageShape
					int frameRow = frameIndex / actualNode->GetFrames();
					int frameColumn = frameIndex % actualNode->GetFrames();
					int cellWidth = (int)(spriteSheet->getWidth() / actualNode->GetFrames());
					int cellHeight = (int)(spriteSheet->getHeight() / actualNode->GetLines());

					ofRectangle imageBound((float)(frameColumn*cellWidth), (float)(frameRow*cellHeight), (float)cellWidth, (float)cellHeight);
					owner->ChangeAttr(ATTR_IMGBOUNDS, imageBound);
					owner->GetMesh<Image>()->SetImage(spriteSheet);

					if (owner->HasMeshType(MeshType::IMAGE)) {
						owner->GetMesh<Image>()->SetImage(spriteSheet);
					}
					else if (owner->HasMeshType(MeshType::NONE)) {
						// set the first image
						owner->SetMesh(spt<Image>(new Image(spriteSheet)));
					}
				}
			}
			else {
				// image is a common image (one sprite per image)
				if (owner->HasAttr(ATTR_IMGBOUNDS)) owner->RemoveAttr(ATTR_IMGBOUNDS, true);

				string imagePath = actualNode->GetSheet(actualIndex);
				spt<ofImage> image = CogGet2DImage(imagePath);
				if (owner->HasMeshType(MeshType::IMAGE)) {
					owner->GetMesh<Image>()->SetImage(image);
				}
				else {
					owner->SetMesh(spt<Image>(new Image(image)));
				}
			}
		}
	}
}// namespace