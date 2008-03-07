<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../tools/rbuild/project.dtd">
<module name="wmc" type="buildtool" allowwarnings="true">
	<define name="WINE_UNICODE_API">" "</define>
	<include base="unicode">.</include>
	<include base="ReactOS">include/reactos/wine</include>
	<include base="ReactOS">include/reactos</include>
	<include base="ReactOS">include</include>
	<include base="ReactOS" root="intermediate">include</include>
	<library>unicode</library>
	<file>lang.c</file>
	<file>mcl.c</file>
	<file>utils.c</file>
	<file>wmc.c</file>
	<file>write.c</file>
	<file>mcy.tab.c</file>
</module>
