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
        <xsl:for-each select="ä®íËâ»ñ⁄/ï‚èïâ»ñ⁄">
          <xsl:element name="OPTION">
            <xsl:if test="ñºèÃ=$selected">
              <xsl:attribute name="SELECTED">true</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="ñºèÃ"/>
          </xsl:element>
        </xsl:for-each>
      </xsl:element>
      <xsl:text>ã‡äz</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_money</xsl:attribute>
        <xsl:attribute name="SIZE">12</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ã‡äz"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_kind</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ãÊï™"/></xsl:attribute>
      </xsl:element>
      <xsl:text>ê≈ãÊï™ÅF</xsl:text>
      <xsl:value-of select="ä®íËâ»ñ⁄/ê≈ãÊï™"/>
      <xsl:text>&nbsp;</xsl:text>
      <xsl:if test="ä®íËâ»ñ⁄/ê≈ãÊï™='â€ê≈édì¸'">
        <xsl:text>ê≈äz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈äz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>ê≈ó¶</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ó¶"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="ä®íËâ»ñ⁄/ê≈ãÊï™='â€ê≈édì¸ï‘ä“'">
        <xsl:text>ê≈äz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈äz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>ê≈ó¶</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ó¶"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="ä®íËâ»ñ⁄/ê≈ãÊï™='â€ê≈îÑè„'">
        <xsl:text>ê≈äz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈äz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>ê≈ó¶</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ó¶"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="ä®íËâ»ñ⁄/ê≈ãÊï™='â€ê≈îÑè„ï‘ä“'">
        <xsl:text>ê≈äz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈äz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>ê≈ó¶</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ó¶"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="ä®íËâ»ñ⁄/ê≈ãÊï™='â€ê≈ë›ì|'">
        <xsl:text>ê≈äz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈äz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>ê≈ó¶</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ê≈ó¶"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <BR/>
      <xsl:text>ìEóv</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_summary</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ìEóv"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text>ÉÅÉÇ</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_memo</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="ä®íËâ»ñ⁄/ÉÅÉÇ"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text>ït‚≥</xsl:text>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tag</xsl:attribute>
        <xsl:element name="OPTION">
          <xsl:if test="ït‚≥='óL'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>óL</xsl:text>
        </xsl:element>
        <xsl:element name="OPTION">
          <xsl:if test="ït‚≥='ñ≥'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>ñ≥</xsl:text>
        </xsl:element>
      </xsl:element>
      <BR/>
  </xsl:template> 

</xsl:stylesheet>
