<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR><TD>���i��</TD><TD>�^��</TD><TD>�d�q�}��</TD><TD>�݌ɐ�</TD></TR>
      <xsl:for-each select="���i�}�X�^�[/���i">
        <xsl:if test="�d�q�}��='No'">
          <TR>
            <TD><xsl:value-of select="���i��"/></TD>
            <TD><xsl:value-of select="�^��"/></TD>
            <TD><xsl:value-of select="�d�q�}��"/></TD>
            <TD align="right"><xsl:value-of select="����"/></TD>
          </TR>
        </xsl:if>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
