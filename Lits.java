import java.util.Arrays;

public class Lits {
    public static void main(String[] args) {
        char data[] = {
            0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 
            0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 
            0x00, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
        };
        char key[] = {0xAA, 0xFF, 0xAA, 0xE, 0xFF, 0xB, 0xAB, 0xEA, 0xB, 0xAA, 0xFA};
        /* System.out.println(Arrays.toString(toByteArray(data)));
        shift(data, key);
        System.out.println(Arrays.toString(toByteArray(data)));
        unshift(data, key);
        System.out.println(Arrays.toString(toByteArray(data)));
        System.out.println(Arrays.toString(toByteArray((key))));
        System.out.println(Arrays.toString(toByteArray(expandKey(key)))); */
        char[] keyres = expandKey(key);
        System.out.println("old: "+Arrays.toString(toByteArray(key)));
        System.out.println("new: "+Arrays.toString(toByteArray(keyres)));
        System.out.println(Arrays.toString(toByteArray(data)));
        char[] result = encrypt(data, key);
        System.out.println(Arrays.toString(toByteArray(result)));
        System.out.println(Arrays.toString(toByteArray(decrypt(result, key))));
    }

    public static char[] encrypt(char[] data, char[] key) {
        char[] keyres = expandKey(key);
        char[] result = new char[data.length];
        result = xor(data, keyres);
        shift(result, keyres);
        return result;
    }
    
    public static char[] decrypt(char[] data, char[] key) {
        char[] result = expandKey(key);
        unshift(data, result);
        data = xor(data, result);
        return data;
    }

    public static char[] xor(char[] data, char[] key) {
        char[] result = new char[data.length];
        for(int x = 0; x < data.length; x++) {
            result[x] = (char) ((char) data[x] ^ key[x % key.length]);
        }
        return result;
    }

    public static void shift(char[] data, char[] key) {
        for(int x = 0; x < data.length; x++) {
            int newshiftidx = (int) x % key.length;
            int newshift = data[newshiftidx];
            int origin = (int) data[x];
            data[newshiftidx] = (char) origin;
            data[x] = (char) newshift;
        }
    }

    public static void unshift(char[] data, char[] key) {
        for(int x = data.length; x-- > 0 ;) {
            int newshiftidx = (int) x % key.length;
            int newshift = data[newshiftidx];
            int origin = (int) data[x];
            data[newshiftidx] = (char) origin;
            data[x] = (char) newshift;
        }
    }

    public static byte[] toByteArray(char[] data) {
        byte[] result = new byte[data.length];
        for(int x = 0; x < data.length; x++) {
            result[x] = (byte) data[x];
        }
        return result;
    }

    public static char[] expandKey(char[] key) {
        char[] newkey = new char[key.length*2];
        for(int x = 0; x < key.length; x++) {
            newkey[x * 2] = key[x];
            int val = (int) key[x];
            int point = (int) key[val % key.length];
            int bound = (int) key[point % key.length];
            if(bound == 0) bound = 1;
            if(point == 0) point = 1;
            int tmp = point;
            for(int o = x; o != point; o += (x < point ? 1 : -1)) {
                int pox = val ^ (key[o % key.length] ^ bound);
                tmp ^= pox;
            }
            newkey[x * 2 + 1] = (char) modPow(tmp, point, bound);
        }
        return newkey;
    }

    private static int modPow(int base, int exp, int mod) {
        int result = 1;
        while (exp > 0) {
            if ((exp & 1) == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }
}