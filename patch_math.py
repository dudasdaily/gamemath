import re

with open("Source/Runtime/Math/Public/MathUtil.h", "r", encoding="euc-kr", errors="ignore") as f:
    content = f.read()

pattern = re.compile(r"(\tFORCEINLINE static float InvSqrt\(float InFloat\)\n\t\{)(.*?)(return temp;\n\t\})", re.DOTALL)
replacement = r"""\1
#if defined(_MSC_VER)\2\3
#else
		return 1.0f / sqrtf(InFloat);
	}
#endif"""

new_content = pattern.sub(replacement, content)
with open("Source/Runtime/Math/Public/MathUtil.h", "w", encoding="utf-8") as f:
    f.write(new_content)

