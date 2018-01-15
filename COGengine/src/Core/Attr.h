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
		bool isPointer;
		void* rawVal; // pointer to raw value
		bool isShared = false;

	public:

		Attr(unsigned key, bool isPointer, void* rawVal, Node* owner, bool isShared)
			: owner(owner), key(key), isPointer(isPointer), rawVal(rawVal), isShared(isShared) {

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

		bool IsPointer() const {
			return isPointer;
		}

		void* RawVal() const {
			return rawVal;
		}
	};


	/**
	* Helper that deletes only pointers
	*/
	template<typename T> class AttrDeleter {

	public:

		static void Destroy(T &value) {
		}
	};

	template<typename P>
	class AttrDeleter<P*> {

	public:

		static void Destroy(P *value) {
			delete value;
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

		/**
		* Creates a new attribute wrapper
		* @param key attribute identifier
		* @param val attribute value
		* @param owner owner node
		*/
		AttrR(unsigned key, T val, Node* owner, bool isShared = false)
			: Attr(key, std::is_pointer<T>(), 0, owner, isShared), value(val) {
			// must be set when its address is determined
			rawVal = &value;
		}

		~AttrR() {
			// remove dynamic attribute
			if (this->isPointer && !isShared) {
				AttrDeleter<T>::Destroy(value);
			}
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