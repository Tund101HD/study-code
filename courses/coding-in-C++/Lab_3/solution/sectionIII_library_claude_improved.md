# Section III – Library UML: Errors Found & Corrected Design

## Errors in the Original Diagram

### Error 1 (Critical): `Librarian` inherits from `Member`
The original shows `Librarian --|> Member`. This is semantically wrong — a Librarian is *not* a subtype of Member. They are completely separate roles in the system. A Member borrows books; a Librarian registers members and manages the collection. Modeling Librarian as a specialisation of Member conflates two orthogonal concepts.

**Fix:** Both `Member` and `Librarian` share the attribute `name`, which signals they should both extend a common base class `Person`.

### Error 2: All `Book` attributes are `public`
`title`, `isbn`, and `available` are all marked `+`. This violates encapsulation — external code should not be able to freely modify a book's availability flag or ISBN directly.

**Fix:** All attributes become `private` (`-`), accessed through methods.

### Error 3: `Librarian.salary` is `public`
Salary is sensitive data and should not be directly readable/writable from outside the class.

**Fix:** `- salary: double`

### Error 4 (Critical): `Loan` composition with `Book`
The original shows a filled diamond (composition) from `Loan` to `Book`, meaning "a Loan owns a Book and the Book ceases to exist when the Loan ends." This is obviously wrong — returning a book does not destroy the book. A Loan *references* a Book; it does not own it.

**Fix:** Replace the composition with a plain association (`Loan --> Book`).

### Error 5: No `Person` base class
Both `Member` and `Librarian` carry a `name` attribute. The absence of a shared base causes duplication and prevents polymorphic treatment of system users.

**Fix:** Introduce `Person` as an abstract base class.

### Error 6: Attribute redundancy
`Library` lists `books: Book[1000]` as a field *and* shows a composition arrow to `Book`. In UML, the relationship arrow already expresses ownership and multiplicity — keeping the attribute too is redundant clutter.

**Fix:** Remove the attribute; let the composition arrow carry the multiplicity.

---

## Remaining Issues in ChatGPT's "Improved" Version

1. Still keeps `- books: Book[1000]` as an attribute inside `Library` alongside the composition arrow — redundancy not fixed.
2. `Loan` does not reference `Member` — a loan record that doesn't know *who* borrowed the book is useless; only a one-way `Member → Loan` association is shown.
3. `Library` has no relationship to `Loan` at all — loan records belong to and are managed by the library system and should be modelled as a composition.
4. `Book` is not marked `<<abstract>>` despite having concrete subtypes `PrintedBook` and `EBook`; you should never instantiate a raw `Book`.

---

## Corrected Design

```mermaid
classDiagram
    direction TB

    class Person {
        <<abstract>>
        # name: string
        # email: string
        + getName() string
    }

    class Member {
        - memberId: int
        + borrow(book: Book) Loan
        + returnBook(loan: Loan) void
    }

    class Librarian {
        - employeeId: int
        - salary: double
        + registerMember(m: Member) void
    }

    class Library {
        - name: string
        + addBook(book: Book) bool
        + addMember(member: Member) void
        + createLoan(member: Member, book: Book) Loan
    }

    class Book {
        <<abstract>>
        - title: string
        - isbn: string
        - available: bool
        + borrow() bool
        + returnBook() void
        + isAvailable() bool
    }

    class PrintedBook {
        - shelfCode: string
        - pageCount: int
        + printLocation() void
    }

    class EBook {
        - fileSizeMB: double
        - downloadUrl: string
        + download() void
    }

    class Loan {
        - loanDate: string
        - dueDate: string
        - returned: bool
        + close() void
        + isOverdue() bool
    }

    Person <|-- Member
    Person <|-- Librarian
    Book <|-- PrintedBook
    Book <|-- EBook

    Library "1" *-- "0..*" Book : owns
    Library "1" *-- "0..*" Loan : manages
    Library "1" o-- "0..*" Member : has registered
    Library "1" --> "1..*" Librarian : employs

    Loan "0..*" --> "1" Member : borrowed by
    Loan "0..*" --> "1" Book : references
```

---

## Design Justification

| Relationship | Type | Reasoning |
|---|---|---|
| `Library` → `Book` | **Composition** | Books are entries in the library's catalogue. Their lifecycle is tied to the library system. |
| `Library` → `Loan` | **Composition** | Loan records are created and owned by the library. No loan exists outside the library context. |
| `Library` → `Member` | **Aggregation** | Members are registered *at* the library but exist as people independently; they could deregister without being destroyed. |
| `Library` → `Librarian` | **Association** | Librarians are employed by the library but have an independent existence (they can change jobs). |
| `Loan` → `Member` | **Association** | A Loan record references the Member who borrowed — navigation is needed to answer "who has this loan?" |
| `Loan` → `Book` | **Association** | A Loan references the borrowed Book. The Book outlives the Loan, so there is no ownership. |
| `Member`, `Librarian` → `Person` | **Inheritance** | Both roles share identifying information (name, email) and differ only in their responsibilities. |
| `PrintedBook`, `EBook` → `Book` | **Inheritance** | Both are specialisations of a book concept, differing only in their physical/digital properties. `Book` is abstract because you never borrow a generic "book" — you borrow a specific kind. |
