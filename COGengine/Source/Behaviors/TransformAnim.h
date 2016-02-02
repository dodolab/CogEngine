#pragma once

#include "Behavior.h"
#include "Node.h"
#include "TransformEnt.h"
#include "TransformMath.h"
#include "ResourceCache.h"
#include "Scene.h"

namespace Cog {

	/**x
	* Behavior for common transformation animation
	*/
	class TransformAnim : public Behavior {
		OBJECT_PROTOTYPE_INIT(TransformAnim)
	private:
		// duration in ms
		float duration = 0;
		// starting transform
		spt<TransformEnt> to = spt<TransformEnt>();
		Trans toTrans;
		// ending transform
		spt<TransformEnt> from = spt<TransformEnt>();
		Trans fromTrans;
		// actual duration
		float actual = 0;
		// fade function
		FadeFunction fadeFunction = nullptr;
		float delayAfter = 0;
		bool repeat = false;

	public:

		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, float delayAfter, bool repeat) : to(to), from(from), duration(duration),
			delayAfter(delayAfter), repeat(repeat) {
		}

		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, float delayAfter, bool repeat, FadeFunction fadeFunction) :
			to(to), from(from), duration(duration), delayAfter(delayAfter), repeat(repeat), fadeFunction(fadeFunction) {
		}

		TransformAnim(Setting setting) {
			string from = setting.GetItemVal("from");
			string to = setting.GetItemVal("to");
			this->duration = setting.GetItem("duration").GetValFloat();
			this->delayAfter = setting.GetItem("delayAfter").GetValFloat();
			this->repeat = setting.GetItemValBool("repeat");

			auto resCache = GETCOMPONENT(ResourceCache);
			this->from = resCache->GetEntityC<TransformEnt>(from);
			this->to = resCache->GetEntityC<TransformEnt>(to);
		}

		void Init() {
			TransformMath math = TransformMath();

			Settings& sceneSettings = owner->GetScene()->GetSettings();
			int gridWidth = sceneSettings.GetSettingValInt("transform", "grid_width");
			int gridHeight = sceneSettings.GetSettingValInt("transform", "grid_height");


			if(from) math.CalcTransform(fromTrans, owner, owner->GetParent(), *from, gridWidth,gridHeight);
			else fromTrans = owner->GetTransform();

			if(to) math.CalcTransform(toTrans, owner, owner->GetParent(), *to, gridWidth, gridHeight);
			else toTrans = owner->GetTransform();
		}

		void Update(const uint64 delta, const uint64 absolute) {

			actual += delta;

			float actualCropped = actual;

			if (actual >= duration) {
				actualCropped = duration;
			}

			float actualPercent = actualCropped / duration;
			if (fadeFunction != nullptr) actualPercent = fadeFunction(actualPercent);


			Trans actualTrans(0, 0);
			actualTrans.localPos = fromTrans.localPos + (toTrans.localPos - fromTrans.localPos)*actualPercent;
			actualTrans.rotation = fromTrans.rotation + (toTrans.rotation - fromTrans.rotation)*actualPercent;
			actualTrans.scale = fromTrans.scale + (toTrans.scale - fromTrans.scale)*actualPercent;
			actualTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
			owner->SetTransform(actualTrans);

			if (actual >= (duration + delayAfter)) {
				if (repeat) {
					actual = 0;
				}
				else {
					Finish();
					SendMessageNoBubbling(ACT_TRANSFORM_ENDED, 0, nullptr, owner);
				}
			}
		}

	};

}// namespace