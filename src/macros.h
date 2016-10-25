#ifndef MACROS_H
#define MACROS_H

#define CNT_ARGS(...) CNT_ARGS_(, ## __VA_ARGS__,8,7,6,5,4,3,2,1,0)
#define CNT_ARGS_(_0,_1,_2,_3,_4,_5,_6,_7,_8,n,...) n

#define REVERSE(...) REVERSE_(CNT_ARGS(__VA_ARGS__),__VA_ARGS__)
#define REVERSE_(n,...) REVERSE__(n,__VA_ARGS__)
#define REVERSE__(n,...) REVERSE_##n(__VA_ARGS__)

#define DROP_TYPES(...) DROP_TYPES_(CNT_ARGS(__VA_ARGS__),__VA_ARGS__)
#define DROP_TYPES_(n,...) DROP_TYPES__(n,__VA_ARGS__)
#define DROP_TYPES__(n,...) DROP_TYPES_##n(__VA_ARGS__)

#define GEN_TYPE_NAME_PAIRS(...) GEN_TYPE_NAME_PAIRS_(CNT_ARGS(__VA_ARGS__), REVERSE(__VA_ARGS__))
#define GEN_TYPE_NAME_PAIRS_(n,...) GEN_TYPE_NAME_PAIRS__(n,__VA_ARGS__)
#define GEN_TYPE_NAME_PAIRS__(n,...) GEN_TYPE_NAME_PAIRS_##n(__VA_ARGS__)

#define INTERSPERSE_SPACE(...) INTERSPERSE_SPACE_(CNT_ARGS(__VA_ARGS__),__VA_ARGS__)
#define INTERSPERSE_SPACE_(n,...) INTERSPERSE_SPACE__(n,__VA_ARGS__)
#define INTERSPERSE_SPACE__(n,...) INTERSPERSE_SPACE_##n(__VA_ARGS__)

/* Reverse the variadic arguments list. */
#define REVERSE_0()
#define REVERSE_1(x,...) x
#define REVERSE_2(x,...) REVERSE_1(__VA_ARGS__), x
#define REVERSE_3(x,...) REVERSE_2(__VA_ARGS__), x
#define REVERSE_4(x,...) REVERSE_3(__VA_ARGS__), x
#define REVERSE_5(x,...) REVERSE_4(__VA_ARGS__), x
#define REVERSE_6(x,...) REVERSE_5(__VA_ARGS__), x
#define REVERSE_7(x,...) REVERSE_6(__VA_ARGS__), x
#define REVERSE_8(x,...) REVERSE_7(__VA_ARGS__), x

/* Drop the types in a sequence of type and argument name pairs:
 * DROP_TYPES(Foo, f, Bar, b) -> f, b */
#define DROP_TYPES_0()
#define DROP_TYPES_2(ptype,pname,...) pname
#define DROP_TYPES_4(ptype,pname,...) pname, DROP_TYPES_2(__VA_ARGS__)
#define DROP_TYPES_6(ptype,pname,...) pname, DROP_TYPES_4(__VA_ARGS__)
#define DROP_TYPES_8(ptype,pname,...) pname, DROP_TYPES_6(__VA_ARGS__)

/* Generate type-and-name pairs in a function declaration parameter list:
 * GEN_TYPE_NAME_PAIRS(Foo, Bar) -> Foo, arg0, Bar, arg1 */
#define GEN_TYPE_NAME_PAIRS_0()
#define GEN_TYPE_NAME_PAIRS_1(ptype,...) ptype, arg0
#define GEN_TYPE_NAME_PAIRS_2(ptype,...) GEN_TYPE_NAME_PAIRS_1(__VA_ARGS__), ptype, arg1
#define GEN_TYPE_NAME_PAIRS_3(ptype,...) GEN_TYPE_NAME_PAIRS_2(__VA_ARGS__), ptype, arg2
#define GEN_TYPE_NAME_PAIRS_4(ptype,...) GEN_TYPE_NAME_PAIRS_3(__VA_ARGS__), ptype, arg3

/* Intersperse spaces between each pair:
 * INTERSPERSE_SPACE(Foo, f, Bar, b) -> Foo f, Bar b */
#define INTERSPERSE_SPACE_0()
#define INTERSPERSE_SPACE_2(ptype, pname,...) ptype pname
#define INTERSPERSE_SPACE_4(ptype, pname,...) ptype pname, INTERSPERSE_SPACE_2(__VA_ARGS__)
#define INTERSPERSE_SPACE_6(ptype, pname,...) ptype pname, INTERSPERSE_SPACE_4(__VA_ARGS__)
#define INTERSPERSE_SPACE_8(ptype, pname,...) ptype pname, INTERSPERSE_SPACE_6(__VA_ARGS__)

#define GEN_APPLICATION_LIST(...) DROP_TYPES(GEN_TYPE_NAME_PAIRS(__VA_ARGS__))
#define GEN_PARAM_LIST(...) INTERSPERSE_SPACE(GEN_TYPE_NAME_PAIRS(__VA_ARGS__))

/*
 * Macro for defining callbacks (`name'_cb) to a function named `name'. The
 * callback captures `this' object. It declares both the function returning the
 * callback and the function that the callback executes.
 */
#define REGISTER_CALLBACK(name, ...) \
	void name(__VA_ARGS__); \
	\
	std::function<void(__VA_ARGS__)> name ## _cb() { \
		return [this](GEN_PARAM_LIST(__VA_ARGS__)) { this->name(GEN_APPLICATION_LIST(__VA_ARGS__)); }; \
	}

// Test-case, uncomment to try out.
//CNT_ARGS()
//CNT_ARGS(1)
//CNT_ARGS(1,2)
//REGISTER_CALLBACK(onMoved, Logic&, Tetro)
//REGISTER_CALLBACK(onDropped)

#endif /* MACROS_H */
