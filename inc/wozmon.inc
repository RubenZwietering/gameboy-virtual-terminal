if !def(WOZMON_INC)
def WOZMON_INC equ 1

if def(WOZMON_OPTIMIZED)
	def WOZMON equs "wozmon"
elif def(WOZMON_ORIGINAL)
	def WOZMON equs "RESET"
endc

endc ; WOZMON_INC