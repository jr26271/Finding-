class Person(var age: Int) {
}

/*
fun set_age(age: Int, p: Person) {
    p.age = age
}

fun get_age(p: Person): Int {
    return p.age
}
*/
fun main() {
    val p = Person(40)
    assert(p.age == 40)
    //set_age(30, p)
    //assert(get_age(p) == 30)
}