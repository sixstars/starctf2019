// written in C++14 by garzon
// $ g++ -std=c++14 -O0 obfuscating_macros.cpp && strip a.out && mv a.out obfuscating_macros.out

#include <unordered_map>
#include <string>
#include <iostream>
#include <queue>
#include <stack>

// a marker for useless(obfuscating) code, w. or w/o. code inside will not affect the logic
#define _USELESS_CODE(...) __VA_ARGS__
// #define _USELESS_CODE(...)

_USELESS_CODE(
	constexpr uint64_t myXorConst = 0x78abda5f;
	constexpr uint64_t myOffsetConst = 0x57419f8e;

	template<uint64_t n>
	struct ConstInt {
		static constexpr uint64_t encoder() {
			constexpr uint64_t i = n + myOffsetConst;
			return i ^ myXorConst;
		}
		static constexpr uint64_t encoded = encoder();
	};

	inline uint64_t ConstInt_decoder(const uint64_t n) {
		return (n^myXorConst)-myOffsetConst;
	}
)

#define _OBF_CONST(c) _USELESS_CODE(ConstInt_decoder) ( _USELESS_CODE(ConstInt<) c _USELESS_CODE(>::encoded) )

#define _GOTO_EXECUTE_FSM _USELESS_CODE(if (!__is_executing)) goto *__goto_labels[__exec_fsm_id]
#define _GOTO_BLOCK(id) __next_state_id.push(id); _GOTO_EXECUTE_FSM
#define _STATE(id) st_ ## id
#define _FUNCTION_START(dummy, id, retVal) { auto &__retVal = (retVal); std::stack<uint64_t> __next_state_id; uint64_t __else_block = 0, __is_executing = dummy & 0, __exec_fsm_id; std::unordered_map<uint64_t, void *> __goto_labels; void *__nxt_ptr; constexpr uint64_t __fsm_entry_id = id; __goto_labels[id] = &&_STATE(id); if (__is_executing) { _STATE(id):
#define FUNCTION_START(retVal)    _FUNCTION_START(__COUNTER__, __COUNTER__, retVal)
#define _FUNCTION_END(id, id2) goto *__goto_labels[0]; } __exec_fsm_id = id; __goto_labels[id]=&&_STATE(id); __goto_labels[0]=&&_STATE(id2); goto *__goto_labels[__fsm_entry_id]; _STATE(id): __nxt_ptr = __goto_labels[__next_state_id.top()]; __next_state_id.pop(); goto *__nxt_ptr; _STATE(id2):; }
#define FUNCTION_END      _FUNCTION_END(__COUNTER__, __COUNTER__)
#define RETURN(...)       __retVal = (__VA_ARGS__); goto *__goto_labels[0]
#define _MAKE_BLOCK_WITH_OID(oid, id, id2, ...) _GOTO_BLOCK(id2); } __goto_labels[oid] = &&_STATE(id); __goto_labels[_OBF_CONST(id2)] = &&_STATE(id2); if (__is_executing) { _STATE(id): __VA_ARGS__; } if (__is_executing) { _STATE(id2): 
#define _MAKE_BLOCK(id, id2, ...) _MAKE_BLOCK_WITH_OID(id, id, id2, __VA_ARGS__)
#define _BLOCK_WITH_NAME(id, name, ...) _GOTO_BLOCK(id); } __goto_labels[_OBF_CONST(id)] = &&_STATE(name); if (__is_executing) { _STATE(name): __VA_ARGS__;
#define _BLOCK(id, ...) _BLOCK_WITH_NAME(id, id, __VA_ARGS__)
#define BLOCK(...) _BLOCK(__COUNTER__, __VA_ARGS__)
//#define BLOCK_WITH_NAME(name, ...) _BLOCK_WITH_NAME(__COUNTER__, name, __VA_ARGS__) BLOCK();
#define MAKE_BLOCK_WITH_NAME(name, ...) _MAKE_BLOCK_WITH_OID(__COUNTER__, name, __COUNTER__, __VA_ARGS__; _GOTO_EXECUTE_FSM)
#define _INCLUDE_BLOCK(name, id1) __next_state_id.push(id1); goto name; _BLOCK_WITH_NAME(id1, id1, )
#define INCLUDE_BLOCK(name) _INCLUDE_BLOCK(_STATE(name), __COUNTER__)
//#define INCLUDE_BLOCK_POINTER(block_ptr) _INCLUDE_BLOCK(*(block_ptr), __COUNTER__)
#define GET_BLOCK_POINTER(name) (&&_STATE(name))

#define _IF(expr, id, id2, id3, id4, ...) _MAKE_BLOCK(id, id2, __VA_ARGS__; _GOTO_BLOCK(id3)) _GOTO_BLOCK(id4); _MAKE_BLOCK(id4, id3, uint64_t __tmp = id3; if (expr) { __tmp -= 2; static_assert(id3 - 2 == id, "IF block id not match"); } _GOTO_BLOCK(__tmp))
#define IF(expr, ...) _IF(expr, __COUNTER__, __COUNTER__, __COUNTER__, __COUNTER__, __VA_ARGS__)
	
#define _WHILE(expr, id, id2, id3, id4, ...) _MAKE_BLOCK(id, id2, __VA_ARGS__; _GOTO_BLOCK(id3)) _GOTO_BLOCK(id3); _MAKE_BLOCK(id3, id4, uint64_t __tmp = id4; if (expr) { __tmp -= 3; static_assert(id4 - 3 == id, "WHILE block id not match"); } _GOTO_BLOCK(__tmp))
#define WHILE(expr, ...) _WHILE(expr, __COUNTER__, __COUNTER__, __COUNTER__, __COUNTER__, __VA_ARGS__)
#define FOR(init, cond, end, ...) BLOCK(init); _WHILE(cond, __COUNTER__, __COUNTER__, __COUNTER__, __COUNTER__, __VA_ARGS__; end)

#define _IF_ELSE(expr, id, id2, id3, id4, id5, ...) _MAKE_BLOCK(id, id2, __VA_ARGS__; _GOTO_BLOCK(id3)) _GOTO_BLOCK(id4); _MAKE_BLOCK(id4, id3, uint64_t __tmp = id5; if (expr) { __tmp -= 4; } static_assert(id5 - 4 == id, "IF_ELSE block id not match"); _GOTO_BLOCK(__tmp); _USELESS_CODE(__else_block+=2;) } _USELESS_CODE(else) { __else_block = id5 )
#define IF_ELSE(expr, ...) _IF_ELSE(expr, __COUNTER__, __COUNTER__, __COUNTER__, __COUNTER__, __COUNTER__, __VA_ARGS__)

#define _ELSE(id5__STATE_id_str, id6, ...) _MAKE_BLOCK_WITH_OID(__else_block, id5__STATE_id_str, id6, __VA_ARGS__; _GOTO_BLOCK(id6))
#define ELSE(...) _ELSE(__COUNTER__, __COUNTER__, __VA_ARGS__)

#define ELIF(expr, ...) ELSE(IF(expr, __VA_ARGS__))
#define _ELIF_ELSE(expr, id, id2, id3, ...) _MAKE_BLOCK_WITH_OID(__else_block, id, id2, IF_ELSE(expr, __VA_ARGS__; _GOTO_BLOCK(id2)) ELSE (_GOTO_BLOCK(id3)); _USELESS_CODE(__else_block++;) } _USELESS_CODE(else) { __else_block = id3 )
#define ELIF_ELSE(expr, ...) _ELIF_ELSE(expr, __COUNTER__, __COUNTER__, __COUNTER__, __VA_ARGS__)

// magic ends ----------------------------------------

constexpr uint64_t highestBit = 0x8000000000000000ULL;
std::pair<uint64_t, uint64_t> box(uint64_t a, uint64_t b) {
	size_t i; 
	
	// this stack-based VM consists of:
	// - two general purpose registers `a`, `b`
	// - a temporary register `tmp` to store the result of operations
	// - a paired-register `tmp_ret`
	std::pair<uint64_t, uint64_t> tmp_ret;
	std::stack<uint64_t> s; // stack of a stack-based VM
	uint64_t tmp; // temporary register
	
	FUNCTION_START(tmp_ret)
		// PUSHs and POPs
		MAKE_BLOCK_WITH_NAME(push_a, s.push(a));
		MAKE_BLOCK_WITH_NAME(push_b, s.push(b));
		MAKE_BLOCK_WITH_NAME(push_tmp, s.push(tmp));
		MAKE_BLOCK_WITH_NAME(push_ab, 
			BLOCK(s.push(a));
			BLOCK(s.push(b));
		);
		MAKE_BLOCK_WITH_NAME(push_ret, 
			BLOCK(s.push(tmp_ret.first));
			BLOCK(s.push(tmp_ret.second));
		);
		MAKE_BLOCK_WITH_NAME(pop, s.pop());
		MAKE_BLOCK_WITH_NAME(pop_ab,
			BLOCK(b = s.top());
			INCLUDE_BLOCK(pop);
			BLOCK(a = s.top());
			INCLUDE_BLOCK(pop);
		);
		MAKE_BLOCK_WITH_NAME(get_ret_val,
			INCLUDE_BLOCK(push_ret);
			INCLUDE_BLOCK(pop_ab);
		);
		MAKE_BLOCK_WITH_NAME(get_top, tmp = s.top());
		MAKE_BLOCK_WITH_NAME(copy, s.push(s.top()));
		
		// Arithmetic and Bit-wise operations, after this `tmp` will be changed
		MAKE_BLOCK_WITH_NAME(add,
			INCLUDE_BLOCK(get_top);
			INCLUDE_BLOCK(pop);
			tmp += s.top();
			INCLUDE_BLOCK(pop);
		);
		MAKE_BLOCK_WITH_NAME(xor, 
			INCLUDE_BLOCK(get_top);
			INCLUDE_BLOCK(pop);
			tmp ^= s.top();
			INCLUDE_BLOCK(pop);
		);
		MAKE_BLOCK_WITH_NAME(not_a,
			tmp = ~a;
		);
		MAKE_BLOCK_WITH_NAME(shl_ab,
			BLOCK(
				tmp = a & highestBit;
				INCLUDE_BLOCK(push_tmp);
			);
			BLOCK(
				a <<= 1;
				tmp = b & highestBit;
			);
			BLOCK(
				IF (tmp, a |= 1);
				b <<= 1;
			);
			INCLUDE_BLOCK(get_top);
			IF (tmp, b |= 1);
			INCLUDE_BLOCK(pop);
		);
		
		// higher level operations
		MAKE_BLOCK_WITH_NAME(b_apb,  // (a, b) = (b, a+b)
			tmp_ret = {b, b};
			INCLUDE_BLOCK(push_ab);
			INCLUDE_BLOCK(add);
			tmp_ret.second = tmp;
			INCLUDE_BLOCK(get_ret_val);
		);
		MAKE_BLOCK_WITH_NAME(box1,
			INCLUDE_BLOCK(not_a);
			INCLUDE_BLOCK(push_tmp);
			IF_ELSE (a&1,
				INCLUDE_BLOCK(push_ab);
			) ELSE (
				INCLUDE_BLOCK(copy);
				INCLUDE_BLOCK(push_b);
			);
			INCLUDE_BLOCK(xor);
			INCLUDE_BLOCK(push_tmp);
			INCLUDE_BLOCK(pop_ab);
		);
		
		// main logic
		FOR(i=0, i<1024, i++,
			INCLUDE_BLOCK(box1);
			INCLUDE_BLOCK(shl_ab);
			INCLUDE_BLOCK(b_apb);
			INCLUDE_BLOCK(shl_ab);
		);
	FUNCTION_END;
	
	return {a, b};
}

/* flag: *CTF{fUnfl@tCf9}
python solution:

highestBit = 0x8000000000000000
mask = highestBit*2-1

def reverse_box1(a, b):
	not_a = a
	if a&1:
		not_a_xor_b = b
		b = not_a_xor_b ^ not_a
		a = mask ^ not_a
	else:
		a_xor_b = b
		a = mask ^ not_a
		b = a_xor_b ^ a
	return (a, b)
	
def reverse_equation(b, apb):
	a = (apb-b)&mask
	return (a, b)

def reverse_shl_ab(a, b):
	b_high = a&1
	a_high = b&1
	b = ((b >> 1) & mask) | (highestBit if b_high else 0)
	a = ((a >> 1) & mask) | (highestBit if a_high else 0)
	return (a, b)
	
a, b = (11666725874628474674, 5810449208445420706)
for i in xrange(1024):
	a, b = reverse_shl_ab(a, b)
	a, b = reverse_equation(a, b)
	a, b = reverse_shl_ab(a, b)
	a, b = reverse_box1(a, b)
	
print hex(a)[2:-1].decode('hex')[::-1] + hex(b)[2:-1].decode('hex')[::-1]
*/

int main() {
	std::string input;
	uint64_t a, b;
	std::pair<uint64_t, uint64_t> res;
	std::cin >> input;
	if (input.length() != 16) {
		goto wrong;
	}
	
	a = *((uint64_t*) input.c_str());
	b = *((uint64_t*) (input.c_str()+8));
	//std::cin >> a >> b;
	res = box(a, b);
	
	std::cout << res.first << " " << res.second << std::endl;
	//return 0;
	if (res.first != 11666725874628474674ULL || res.second != 5810449208445420706ULL) {
		goto wrong;
	}
	std::cout << "Congratulations!" << std::endl;
	return 0;
	
wrong:
	std::cout << "Failed" << std::endl;
	return 0;
}