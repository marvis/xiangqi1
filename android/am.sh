# Script to start "am" on the device, which has a very rudimentary
# shell.
#
base=/system
export CLASSPATH=$base/framework/am.jar
exec /system/bin/app_process $base/bin com.android.commands.am.Am "$@"

