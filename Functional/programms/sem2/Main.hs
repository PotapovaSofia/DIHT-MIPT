import System.IO

\x y -> x + y

type Expr = Int | String | Lambda (String Expr) | Op (String, [Expr])

compute Op ("plus", [a,b]) = a + b

let f = Lambda "x" (Lambda "y" Op ("plus", ["x", "y"]))
result = apply f 5

функция принимае на вход все аргументы и вычисляет результаты этого выражения
