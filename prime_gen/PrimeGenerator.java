import java.util.*;

public class PrimeGenerator {
    private int limit_;
    private int increment_;
    private PrimeList oddPrimes_ = new PrimeList();
    private BitSet isComposite_ = new BitSet();

    private static class PrimeList {
        private PrimeList() {
            size_ = 0;
            capacity_ = 16;
            primes_ = new int[2*capacity_];
        }
        private int getPrime(int n) {
            return primes_[n*2];
        }
        private int getMultiple(int n) {
            return primes_[n*2 + 1];
        }
        private void addPrime(int prime, int multiple) {
            if (capacity_ < size_ + 1) {
                int newCapacity = Math.max((3 * capacity_)/2, size_ + 1);
                int[] newArray = new int[2 * newCapacity];
                System.arraycopy(primes_, 0, newArray, 0, size_ * 2);
                capacity_ = newCapacity;
                primes_ = newArray;
            }
            int index = size_ * 2;
            primes_[index] = prime;
            primes_[index + 1] = multiple;
            ++size_;
        }
        private void setMultiple(int n, int value) {
            primes_[n*2 + 1] = value;
        }
        private int size() {
            return size_;
        }
        private int size_;
        private int capacity_;
        private int[] primes_;
    }
    
    // Constructs a prime generator with the given initial limit (which is rounded
    // up to the next odd number).
    // The maximum prime number found is increased as required, in steps
    // determined by the increment parameter.
    public PrimeGenerator(int initialLimit, int increment) {
        limit_ = Math.max(3, 1 + 2*(initialLimit/2));
        increment_ = Math.max(16, 2*((increment+1)/2));
        findPrimes(3);
    }
    
    private void extend() {
        int start = limit_ + 2;
        limit_ += increment_;
        findPrimes(start);
    }
    
    private void findPrimes(int start) {
        // Mark multiples of primes already found as not prime
        for (int i = 0, n = oddPrimes_.size(); i < n; ++i) {
            int prime = oddPrimes_.getPrime(i);
            int multiple = oddPrimes_.getMultiple(i);
            int inc = 2 * prime;
            if (multiple > limit_ + inc)
                break;
            for (; multiple <= limit_; multiple += inc)
                isComposite_.set(multiple/2 - 1, true);
            oddPrimes_.setMultiple(i, multiple);
        }
        // Look for new odd primes <= limit
        int sqrt = (int)Math.sqrt(limit_);
        for (int prime = start; prime <= sqrt; prime += 2) {
            if (!isComposite_.get(prime/2 - 1)) {
                int multiple = prime * prime;
                int increment = 2 * prime;
                for (; multiple <= limit_; multiple += increment)
                    isComposite_.set(multiple/2 - 1, true);
            }
        }
        // Add new odd primes to the list
        for (int prime = start; prime <= limit_; prime += 2) {
            if (!isComposite_.get(prime/2 - 1))
                oddPrimes_.addPrime(prime, prime * prime);
        }
    }
    
    public boolean isPrime(int n) {
        if (n == 2)
            return true;
        if (n < 2 || n % 2 == 0)
            return false;
        while (n > limit_)
            extend();
        return !isComposite_.get(n/2 - 1);
    }

    public int nthPrime(int n) {
        if (n == 1)
            return 2;
        while (n >= oddPrimes_.size() + 2)
            extend();
        return oddPrimes_.getPrime(n - 2);
    }

    public static void main(String[] args) {
        PrimeGenerator pgen = new PrimeGenerator(10, 250000);
        System.out.println("First 20 primes:");
        for (int i = 1; i <= 20; ++i) {
            if (i > 1)
                System.out.print(", ");
            System.out.print(pgen.nthPrime(i));
        }
        System.out.println();
        System.out.println("Primes between 100 and 150:");
        for (int n = 100, i = 0; n <= 150; ++n) {
            if (pgen.isPrime(n)) {
                if (i++ != 0)
                    System.out.print(", ");
                System.out.print(n);
            }
        }
        System.out.println();
        int count = 0;
        for (int n = 7700; n <= 8000; ++n) {
            if (pgen.isPrime(n))
                ++count;
        }
        System.out.println("Number of primes between 7700 and 8000: " + count);
        for (int n = 10000; n <= 100000000; n *= 10)
            System.out.println(n + "th prime: " + pgen.nthPrime(n));
        System.out.println("Memory usage: " + Runtime.getRuntime().totalMemory()/(1024.0*1024.0) + "MB");
    }
}
