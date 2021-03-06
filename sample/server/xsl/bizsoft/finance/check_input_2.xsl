<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:apply-templates select="EBIZ"/>
  </xsl:template> 

  <xsl:template match="EBIZ">
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_selected_ah</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="Selected"/></xsl:attribute>
      </xsl:element>
      <xsl:value-of select="Selected"/>
      <xsl:variable name="selected" select="Selected2"/>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_selected_as</xsl:attribute>
        <xsl:for-each select="¨èÈÚ/âÈÚ">
          <xsl:element name="OPTION">
            <xsl:if test="¼Ì=$selected">
              <xsl:attribute name="SELECTED">true</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="¼Ì"/>
          </xsl:element>
        </xsl:for-each>
      </xsl:element>
      <xsl:text>àz</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_money</xsl:attribute>
        <xsl:attribute name="SIZE">12</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/àz"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_kind</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åæª"/></xsl:attribute>
      </xsl:element>
      <xsl:text>ÅæªF</xsl:text>
      <xsl:value-of select="¨èÈÚ/Åæª"/>
      <xsl:text>&nbsp;</xsl:text>
      <xsl:if test="¨èÈÚ/Åæª='ÛÅdü'">
        <xsl:text>Åz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>Å¦</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Å¦"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="¨èÈÚ/Åæª='ÛÅdüÔÒ'">
        <xsl:text>Åz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>Å¦</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Å¦"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="¨èÈÚ/Åæª='ÛÅã'">
        <xsl:text>Åz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>Å¦</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Å¦"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="¨èÈÚ/Åæª='ÛÅãÔÒ'">
        <xsl:text>Åz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>Å¦</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Å¦"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="¨èÈÚ/Åæª='ÛÅİ|'">
        <xsl:text>Åz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Åz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>Å¦</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Å¦"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <BR/>
      <xsl:text>Ev</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_summary</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/Ev"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text></xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_memo</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="¨èÈÚ/"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text>tâ³</xsl:text>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tag</xsl:attribute>
        <xsl:element name="OPTION">
          <xsl:if test="tâ³='L'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>L</xsl:text>
        </xsl:element>
        <xsl:element name="OPTION">
          <xsl:if test="tâ³='³'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>³</xsl:text>
        </xsl:element>
      </xsl:element>
      <BR/>
  </xsl:template> 

</xsl:stylesheet>
