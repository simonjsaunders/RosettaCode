import java.util.*;

public class PrimeGenerator2 {
    private int limit_;
    private int index_ = 0;
    private int increment_;
    private int count_ = 0;
    private List<Integer> primes_ = new ArrayList<>();
    private Sieve sieve_;
    
    public PrimeGenerator2(int initialLimit, int increment) {
        limit_ = initialLimit;
        increment_ = increment;
        sieve_ = new Sieve(sqrt(limit_));
        primes_.add(2);
        findPrimes(3);
    }
    
    public int nextPrime() {
        if (index_ == primes_.size()) {
            if (Integer.MAX_VALUE - increment_ < limit_)
                return 0;
            int start = 1 + 2*((limit_+1)/2);
            limit_ += increment_;
            sieve_.sieve(sqrt(limit_));
            primes_.clear();
            findPrimes(start);
        }
        ++count_;
        return primes_.get(index_++);
    }
    
    public int count() {
        return count_;
    }
    
    private void findPrimes(int start) {
        index_ = 0;
        BitSet composite = new BitSet();
        for (int p = 3, n = sieve_.limit_; p <= n; p += 2) {
            if (!sieve_.isPrime(p))
                continue;
            int q = p * Math.max(p, (start + p - 1)/p);
            if (q % 2 == 0)
                q += p;
            for (; q >= start && q <= limit_; q += 2*p)
                composite.set((q - start)/2, true);
        }
        for (int p = start; p <= limit_; p += 2) {
            if (!composite.get((p - start)/2))
                primes_.add(p);
        }
    }
    
    private static int sqrt(int n) {
        return 1 + 2 * ((int)Math.sqrt(n)/2);
    }

    private static class Sieve {
        private int limit_ = 0;
        private BitSet composite_ = new BitSet();
        private Sieve(int limit) {
            sieve(limit);
        }
        private void sieve(int newLimit) {
            newLimit = 1 + 2*(newLimit/2);
            for (int p = 3; p * p <= newLimit; p += 2) {
                if (!composite_.get(p/2 - 1)) {
                    int q = p * Math.max(p, (limit_ + p - 1)/p);
                    if (q % 2 == 0)
                        q += p;
                    for (; q <= newLimit; q += 2*p) {
                        composite_.set(q/2 - 1, true);
                    }
                }
            }
            limit_ = newLimit;
        }
        private boolean isPrime(int n) {
            if (n < 2)
                return false;
            if (n % 2 == 0)
                return n == 2;
            return !composite_.get(n/2 - 1);
        }
    }
    
    public static void main(String[] args) {
        PrimeGenerator2 pgen = new PrimeGenerator2(20, 200000);
        System.out.println("First 20 primes:");
        for (int i = 0; i < 20; ++i) {
            if (i > 0)
                System.out.print(", ");
            System.out.print(pgen.nextPrime());
        }
        System.out.println();
        System.out.println("Primes between 100 and 150:");
        for (int i = 0; ; ) {
            int prime = pgen.nextPrime();
            if (prime > 150)
                break;
            if (prime >= 100) {
                if (i++ != 0)
                    System.out.print(", ");
                System.out.print(prime);
            }
        }
        System.out.println();
        int count = 0;
        for (;;) {
            int prime = pgen.nextPrime();
            if (prime > 8000)
                break;
            if (prime >= 7700)
                ++count;
        }
        System.out.println("Number of primes between 7700 and 8000: " + count);
        int n = 10000;
        for (;;) {
            int prime = pgen.nextPrime();
            if (prime == 0) {
                System.out.println("Can't generate any more primes.");
                break;
            }
            if (pgen.count() == n) {
                System.out.println(n + "th prime: " + prime);
                n *= 10;
            }
        }
    }
}
