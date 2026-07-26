#include <stdexcept>

class Allocator
{
private:
    struct Header {
        std::size_t size;
        bool isFree;
        Header * next;
    };

public:
    /* special member functions */
    Allocator(std::size_t cap) : m_capacity(cap) {
         m_buffer = new char[cap];
         m_header = new(m_buffer) Header {  // placeent new, create free header
            cap - sizeof(Header),
            true,
            nullptr};
    }
    ~Allocator() {
        /** */
        delete[] static_cast<char*>(m_buffer);
    }

    // copy semantics deprecates with user-defined dtor
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete; 

    Allocator(Allocator&& rhs) noexcept {
        m_capacity = rhs.m_capacity;
        m_buffer = rhs.m_buffer;
        m_header = rhs.m_header;
        
        rhs.m_capacity = 0;
        rhs.m_buffer = nullptr;
        rhs.m_header = nullptr;
    }
    Allocator& operator=(Allocator&& rhs) {
        if (&rhs == this) return *this;

        m_capacity = rhs.m_capacity;
        m_buffer = rhs.m_buffer;
        m_header = rhs.m_header;

        delete rhs.m_buffer;
        delete rhs.m_header;
        
        rhs.m_capacity = 0;
        rhs.m_buffer = nullptr;
        rhs.m_header = nullptr;
    }

    /* member functions */
    void* allocate(std::size_t size) {
        Header* cur = m_header; // starts at the buffer

        while (cur)
        {
            // should fit
            if (cur->isFree && cur->size >= size) {
                // should split 
                if (cur->size >= size + sizeof(Header) + 1) { // *sizeof(Header) + 1 is requred for the next valid block 
                    void* hdr = cur + 1; // cur + 1 for the next hedaer, then implicit conversion Header* -> void*
                    char* nloc = static_cast<char*>(hdr) + size; // requested data ends at nloc

                    Header* next = new (nloc) Header { // next hader
                        cur->size - size - sizeof(Header),
                        true,
                        cur->next
                    };

                    // update header for the request payload 
                    cur->size = size; 
                    cur->next = next;
                }

                cur->isFree = false;
                return static_cast<void*>(cur + 1); // caller needs ptr to payload (not hdr)
            }
            cur = cur->next;
        }
        throw std::bad_alloc(); // throw if cur = nullptr
    }

    void deallocate(void* p) {
        if (p == nullptr) return;
        
        if (!owns(p)) {
            throw std::invalid_argument("invalid pointer");
        }
        Header* h = static_cast<Header*>(p) - 1;            
        h->isFree = true;

        merge();
    }

    bool owns(const void* p) const {
        if (p == nullptr) return false;

        Header* cur =  m_header;
        while (cur)
        { // walk
            void* user = cur + 1;
            if (user == p && !cur->isFree) return true;
            cur = cur->next;
        }

        return false;
    }

private:
    void merge() {
        Header* cur = m_header;
        while (cur && cur->next)
        {
            Header* next = cur->next;
            if (cur->isFree && next->isFree) {
                cur->size += sizeof(Header) + next->size;
                cur->next = next->next;

                next->~Header();
            }
            else {
                cur = cur->next;
            }
        }
        
    }

private:
    std::size_t m_capacity;
    void* m_buffer; // pointer to buffer
    Header* m_header;

};

