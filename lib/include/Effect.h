#pragma once

namespace fm {
	class Effect {
	public:
		Effect() {}
		virtual ~Effect() {}
		bool isBlooming() {return bloom;}
		void setBloom(bool value) {this->bloom = value;}
	protected:
		bool bloom = false;

	};


}