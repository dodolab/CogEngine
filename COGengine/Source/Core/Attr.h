#pragma once


namespace Cog {

	class Node;

	/**
	* Common class for generic attributes
	*
	*/
	class Attr {
	protected:
		// owner node
		Node* owner;
		// key identifier
		const int key;

	public:

		Attr(int key, Node* owner) : owner(owner), key(key) {

		}

		virtual ~Attr()
		{

		}

		/**
		* Gets owner node
		*/
		const Node* GetOwner() const {
			return owner;
		}

		/**
		* Gets key identifier
		*/
		const int GetKey() const {
			return key;
		}
	};


	/**
	* Attribute generic wrapper
	*
	*/
	template <class  T>
	class AttrR : public Attr {
	protected:
		// generic value
		T value;

	public:

		~AttrR()
		{

		}

		/**
		* Creates a new generic attribute
		* @param key attribute key identifier
		* @param val attribute value
		* @param owner owner node
		*/
		AttrR(int key, T val, Node* owner) : Attr(key, owner), value(val) {

		}

		/**
		* Gets reference to the attribute value
		*/
		T& GetValue() {
			return (value);
		}

		/**
		* Sets attribute value
		*/
		void SetValue(T val) {
			T& temp = value;
			this->value = val;
		}

	};

}// namespace