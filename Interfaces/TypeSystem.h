#pragma once

#define TypeClass()                                                                                                                  \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	public:                                                                                                                          \
		template<class T>                                                                                                            \
		bool isInstanceOf() { return isa(T::Type); }                                                                                 \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) {                                                                                            \
			return otherType == Type;                                                                                                \
		}                                                                                                                            \


#define TypeInterface TypeClass
/*#define TypeInterface()                                                                                                            \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) { return otherType == Type; }                                                                \


#define TypeFirstClassInherit1(iface)                                                                                                \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	public:                                                                                                                          \
		template<class T>                                                                                                            \
		bool isa() { return isa(T::Type); }                                                                                          \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) { return otherType == Type || iface::isa(otherType); }                                       \


#define TypeFirstClassInherit2(iface1, iface2)                                                                                       \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	public:                                                                                                                          \
		template<class T>                                                                                                            \
		bool isaT() { return isa(T::Type); }                                                                                          \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) { return otherType == Type || iface1::isa(otherType) || iface2::isa(otherType);  }           \*/


#define TypeInherit1(ancestor)                                                                                                       \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	public:                                                                                                                          \
		template<class T>                                                                                                            \
		bool isInstanceOf() { return isa(T::Type); }                                                                                 \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) {                                                                                            \
			return otherType == Type || ancestor::isa(otherType);                                                                    \
		}                                                                                                                            \


#define TypeInherit2(ancestor1, ancestor2)                                                                                           \
                                                                                                                                     \
	private:                                                                                                                         \
		static const int Type_ = 0;                                                                                                  \
                                                                                                                                     \
	public:                                                                                                                          \
		static int Type;                                                                                                             \
                                                                                                                                     \
	public:                                                                                                                          \
		template<class T>                                                                                                            \
		bool isInstanceOf() { return isa(T::Type); }                                                                                 \
                                                                                                                                     \
	protected:                                                                                                                       \
		virtual bool isa(int otherType) {                                                                                            \
			return otherType == Type || ancestor1::isa(otherType) || ancestor2::isa(otherType);                                      \
		}                                                                                                                            \


#define TypeSetup(type)                                                                                                              \
	int type::Type = (int)&type::Type_;                                                                                              \

