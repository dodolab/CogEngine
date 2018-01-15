#pragma once


namespace Cog {

	class Node;

	/**
	* Wrapper for node attributes
	*
	*/
	class Attr {
	protected:
		// owner node
		Node* owner;
		// key identifier
		const unsigned key;

	public:

		Attr(unsigned key, Node* owner) : owner(owner), key(key) {

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
		unsigned GetKey() const {
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
		T value;

	public:

		~AttrR()
		{

		}

		/**
		* Creates a new attribute wrapper
		* @param key attribute identifier
		* @param val attribute value
		* @param owner owner node
		*/
		AttrR(unsigned key, T val, Node* owner) 
			: Attr(key, owner), value(val) {

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
		void SetValue(T& val) {
			this->value = val;
		}
	};

}// namespace