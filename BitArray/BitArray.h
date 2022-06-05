//
// Created by xilinx_0 on 3/1/22.
//

#ifndef WALKERNET_BITARRAY_H
#define WALKERNET_BITARRAY_H

#include <memory>
#include <vector>
#include <string>
#include <cstring>

using byte = unsigned char;
using byte_ptr = byte*;
using bytes = std::vector<byte>;


/**
 * BitArray
 * @Author: Walker
 * @Date  : test done! 3/3/2022
 * simulate bits, which can calculate such as and, or, xor, etc. bit calculations.
 * The BitArray can just only handle the network byte order.
 * Tell the truth, the logic is so disgusting.
 */
struct BitArray;
using pBitArray = std::shared_ptr<BitArray>;
struct BitArray {
private:
    bytes value;
    int len;
public:

    /**
     *          BitArray's multiple constructor.
     *          BitArray has so many constructor.
     *          Every constructor has its own application environment.
     */


    /**
     *      create an empty bit array.
     */
    BitArray() {
        value = bytes(0);
        len = 0;
    }


    /**
     * init use vector<byte>, this constructor is used to modify flow table.
     * @param value
     */
    explicit BitArray(const bytes& value) : len(value.size() * 8) {
        this->value = value;
    }


    /**
     * init use string, this function is similar to BitArray(const bytes&)
     * Only change the list from vector<byte> to string.
     * @param value
     */
    explicit BitArray(const std::string& value) : len(value.length() * 8) {
        for (const auto& x : value) {
            this->value.push_back(x);
        }
    }

    /**
     * Copy Constructor.
     * @param v 
     */
    BitArray(const BitArray& v) {
        value = v.value;
        len = v.len;
    }

    /**
     * init use integer, maintain the original value.
     * @param t
     * @param len
     */
    BitArray(uint32_t t, int len): len(len) {
        value = bytes(ByteLength(), 0);
        memcpy(&value[0], &t, (sizeof(uint32_t) < ByteLength()) ? sizeof(uint32_t) : ByteLength());
        ResetLength(len);
    }

    /**
     * init use array, maintain the original value.
     * @param p
     * @param len
     */
    BitArray(byte_ptr p, int len) : len(len) {
        value = bytes(ByteLength(), 0);
        memcpy(&value[0], p, ByteLength());
        ResetLength(len);
    }


    /**
     * get an all-zero bit array which length is len.
     * @param len
     */
    explicit BitArray(int len): len(len) {
        value = bytes(ByteLength(), 0);
    }


    /**
     * init use the net frame.
     * every order of field in net packet is not the order of byte, it's totally reverse.
     * @param b 
     * @param len 
     * @param offset 
     */
    BitArray(byte_ptr bits, int len, int offset) {
        int begin_byte = offset / 8; // which byte the beginning of the header at.
        int bit_offset = offset % 8; // which bit  the beginning of the header at in begin_byte.
        this->len = len + bit_offset;
        value = bytes(ByteLength(), 0);
        for (int i = 0; i < ByteLength(); i++) value[i] = bits[begin_byte + i];
        RightShift(bit_offset);
        ResetLength(len);
        Swap();
    }


    ~BitArray() {
        value.clear();
    }




    /**
     * write to the packet.
     * @param bits
     * @param len
     * @param offset
     */
    void Write(byte_ptr bits, int offset) const {
        BitArray tmp = *this;
        BitArray mask = BitArray(Length());
        for (int i = 0; i < mask.Length(); i++) {
            mask.SetBit(i, 1);
        }
        int begin_byte = offset / 8;
        int bit_offset = offset % 8;
        tmp.Swap();
        tmp.ResetLength(BitArray(tmp.Length() + bit_offset).ByteLength() * 8);
        mask.ResetLength(tmp.Length());
        tmp.LeftShift(bit_offset);
        mask.LeftShift(bit_offset);
        BitArray origin = BitArray(bits + begin_byte, tmp.Length());
        origin = origin & ~(mask) | tmp;
        memcpy(bits + begin_byte, origin.CValue(), origin.ByteLength());

    }


    /**
     * It's calculate by the len, without bytes.size().
     * We can trust len rather than bytes.size()
     * @return
     */
    int ByteLength() const {
        if (len == 0) return 0;
        return len / 8 + (len % 8 != 0);
    }

    /**
     * Transform the bitArray into integer.
     * @return
     */
    uint32_t Integer() const {
        uint32_t integer = 0;
        for (int i = 0; i < ByteLength(); i++) {
            *((byte_ptr)&integer + i) = value[i];
        } return integer;
    }

    /**
     * Combine the target BitArray to itself, it is used to build a key.
     * @param b
     */
    void Combine(BitArray b) {
        int sum = len + b.len;
        ResetLength(sum);
        LeftShift(b.len);
        b.ResetLength(sum);
        Or(b);
    }


    /**
     * calculate engine, it is used to execute the AST.
     */
    static BitArray Calculate(int op, BitArray bits1, BitArray bits2) {
        
        switch (op) {
            case 1: return bits1 << bits2.Integer();
            case 2: return bits2 >> bits2.Integer();
            case 3: return bits1 & bits2;
            case 4: return bits1 | bits2;
            case 5: return bits1 ^ bits2;
            case 6: return ~bits1;
            case 7: return BitArray(bits1.Integer() + bits2.Integer(), std::max(bits1.len, 32));
            case 8: return BitArray(bits1.Integer() - bits2.Integer(), std::max(bits1.len, 32));
            default: return BitArray();
        }
        
    }



    int Length() const {
        return len;
    }

    const bytes& Bytes() const {
        return value;
    }

    byte_ptr Value() {
        return &value[0];
    }

    const char* CValue() const {
        return (const char*)&value[0];
    }


    /**
     * Consistence Maintainer, sometimes the values length may not equal to
     * ByteLength(), so we must reset the values.size() to make sure for
     * consistence.
     * @param new_len
     */
    void ResetLength(int new_len) {
        if (ByteLength() == 0) {
            *this = BitArray(new_len);
            return;
        } else if (new_len == 0) {
            *this = BitArray();
            return;
        }
        if (len < new_len) {
            len = new_len;
            while (value.size() < ByteLength()) {
                value.push_back(0);
            } return;
        } else if (len > new_len) {
            len = new_len;
            while (value.size() > ByteLength()) {
                value.pop_back();
            } value[value.size()-1] &= TailMask();
            return;
        } else {
            value[value.size()-1] &= TailMask();
        }
    }

    BitArray SubBitArray(int len, int offset) const {
        BitArray tmp = *this;
        tmp = tmp >> offset;
        tmp.ResetLength(len);
        return tmp;
    }

    bool operator == (const BitArray& b) const {
        if (len != b.len) return false;
        for (int i = 0; i < ByteLength(); i++) {
            if (value[i] != b.value[i]) {
                return false;
            }
        } return true;
    }

    bool operator != (const BitArray& b) const {
        return !(*this == b);
    }

    BitArray operator >> (int shift) const {

        if (shift >= len) {
            return BitArray(len);
        }
        BitArray res = *this;
        auto& v = res.value;

        int offset = shift % 8;
        if (offset != 0) {
            v[0] >>= offset;
            for (int i = 0; i < v.size() - 1; i++) {
                v[i] |= (v[i + 1] % (1 << offset)) << (8 - offset);
                v[i + 1] >>= offset;
            }
        }

        int k = shift / 8;
        if (k != 0) {
            for (int i = k; i < v.size(); i++) {
                v[i - k] = v[i];
            }
            for (int i = 0; i < k; i++) {
                v[v.size() - 1 - i] = 0;
            }
        }
        res.ResetLength(this->len);
        return res;
    }
    
    BitArray operator << (int shift) const {

        if (shift >= len) {
            return BitArray(len);
        }
        BitArray res = *this;
        auto& v = res.value;

        int offset = shift % 8;
        if (offset != 0) {
            v[v.size() - 1] <<= offset;
            for (int i = v.size() - 1; i > 0; i--) {
                v[i] |= v[i - 1] >> (8 - offset);
                v[i - 1] <<= offset;
            }
        }
        int k = shift / 8;
        if (k != 0) {
            for (int i = v.size() - 1; i >= k; i--) {
                v[i] = v[i - k];
            }
            for (int i = 0; i < k; i++) {
                v[i] = 0;
            }
        }
        res.ResetLength(this->len);
        return res;
    }

    BitArray operator & (BitArray bits) const {
        for (int i = 0; i < ByteLength(); i++) {
            bits.value[i] &= value[i];
        } return bits;
    }
    BitArray operator | (BitArray bits) const {
        for (int i = 0; i < ByteLength(); i++) {
            bits.value[i] |= value[i];
        } return bits;
    }
    BitArray operator ^ (BitArray bits) const {
        for (int i = 0; i < ByteLength(); i++) {
            bits.value[i] ^= value[i];
        } return bits;
    }
    BitArray operator ~ () const {
        BitArray res = *this;
        for (int i = 0; i < ByteLength(); i++) {
            res.value[i] = ~res.value[i];
        } res.ResetLength(res.Length());
        return res;
    }

    void Print(int k=2) const;

    byte GetBit(int i) const {
        int byte_id = i / 8;
        int bit_offset = i % 8;
        return (value[byte_id] & (1 << bit_offset)) > 0;
    }

    void SetBit(int i, int v) {
        int byte_id = i / 8;
        int bit_offset = i % 8;
        if (v == 1) value[byte_id] |= (1 << bit_offset);
        else value[byte_id] &= ~(1 << bit_offset);
    }


    const byte& operator[](int i) const {
        return value[i];
    }

    void Set(int i, int v) {
        value[i] = v;
    }

private:

    static const byte rvs[];

    int TailOffset() const {
        return (TailLength()) ? (8-TailLength()) : 0;
    }

    int TailLength() const {
        return (len % 8);
    }

    byte TailMask() const {
        if (TailLength() == 0) return -1;
        return (1 << TailLength()) - 1;
    }


    /**
     * Swap all the bit.
     */
    void Swap() {
        int tmp_len = len;
        int tailOffset = TailOffset();
        ResetLength(ByteLength() * 8);
        LeftShift(tailOffset);
        for (int i = 0; i < ByteLength(); i++) {
            value[i] = rvs[value[i]];
        }
        for (int i = 0; i < ByteLength() / 2; i++) {
            std::swap(value[i], value[ByteLength() - 1 - i]);
        }
        ResetLength(tmp_len);
    }


    /**
     * For a bitarray, right shift all the bits.
     * @param shift
     */
    void RightShift(int shift) {
        if (shift >= len) {
            for (auto& x : value) x = 0;
            return;
        }
        auto& v = value;

        int offset = shift % 8;
        if (offset != 0) {
            v[0] >>= offset;
            for (int i = 0; i < v.size() - 1; i++) {
                v[i] |= (v[i + 1] % (1 << offset)) << (8 - offset);
                v[i + 1] >>= offset;
            }
        }

        int k = shift / 8;
        if (k != 0) {
            for (int i = k; i < v.size(); i++) {
                v[i - k] = v[i];
            }
            for (int i = 0; i < k; i++) {
                v[v.size() - 1 - i] = 0;
            }
        }
        ResetLength(this->len);
    }



    /**
     * For a Bitarray, Left shift all the bit.
     * @param shift
     */
    void LeftShift(int shift) {
        if (shift >= len) {
            for (auto& x : value) x = 0;
            return;
        }
        auto& v = value;

        int offset = shift % 8;
        if (offset != 0) {
            v[v.size() - 1] <<= offset;
            for (int i = v.size() - 1; i > 0; i--) {
                v[i] |= v[i - 1] >> (8 - offset);
                v[i - 1] <<= offset;
            }
        }
        int k = shift / 8;
        if (k != 0) {
            for (int i = v.size() - 1; i >= k; i--) {
                v[i] = v[i - k];
            }
            for (int i = 0; i < k; i++) {
                v[i] = 0;
            }
        }
        ResetLength(len);
    }

    void And(const BitArray& b) {
        for (int i = 0; i < ByteLength(); i++) {
            value[i] &= b.value[i];
        }
    }

    void Or(const BitArray& b) {
        for (int i = 0; i < ByteLength(); i++) {
            value[i] |= b.value[i];
        }
    }




public:
    static void Test();
    static void GetNetworkByteOrder(byte_ptr ptr, int size);
};


#endif //WALKERNET_BITARRAY_H
