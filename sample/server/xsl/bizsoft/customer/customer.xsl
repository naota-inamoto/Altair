<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR>
        <TD>ê©</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">last_name</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ñºëO/ê©"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ê©ÉtÉäÉKÉi</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">last_name_yomi</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ñºëO/ê©ÉtÉäÉKÉi"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ñº</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">first_name</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ñºëO/ñº"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ñºÉtÉäÉKÉi</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">first_name_yomi</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ñºëO/ñºÉtÉäÉKÉi"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ê´ï </TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">radio</xsl:attribute>
            <xsl:attribute name="NAME">sex</xsl:attribute>
            <xsl:attribute name="VALUE">male</xsl:attribute>
            <xsl:if test="å⁄ãq/ê´ï ='male'">
              <xsl:attribute name="CHECKED">true</xsl:attribute>
            </xsl:if>
          </xsl:element>
          <xsl:text>íj</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">radio</xsl:attribute>
            <xsl:attribute name="NAME">sex</xsl:attribute>
            <xsl:attribute name="VALUE">female</xsl:attribute>
            <xsl:if test="å⁄ãq/ê´ï ='female'">
              <xsl:attribute name="CHECKED">true</xsl:attribute>
            </xsl:if>
          </xsl:element>
          <xsl:text>èó</xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>ê∂îNåéì˙</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">4</xsl:attribute>
            <xsl:attribute name="NAME">birth_year</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ê∂îNåéì˙/îN"/></xsl:attribute>
          </xsl:element>
          <xsl:text>îN</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">2</xsl:attribute>
            <xsl:attribute name="NAME">birth_month</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ê∂îNåéì˙/åé"/></xsl:attribute>
          </xsl:element>
          <xsl:text>åé</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">2</xsl:attribute>
            <xsl:attribute name="NAME">birth_day</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ê∂îNåéì˙/ì˙"/></xsl:attribute>
          </xsl:element>
          <xsl:text>ì˙</xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>óXï÷î‘çÜ</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">7</xsl:attribute>
            <xsl:attribute name="NAME">zip_code</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/èZèä/óXï÷î‘çÜ"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ìsìπï{åß</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">8</xsl:attribute>
            <xsl:attribute name="NAME">prifecture</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/èZèä/ìsìπï{åß"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ésí¨ë∫î‘ínÇP</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address1</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/èZèä/ésí¨ë∫î‘ínÇP"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ésí¨ë∫î‘ínÇQ</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address2</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/èZèä/ésí¨ë∫î‘ínÇQ"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ésí¨ë∫î‘ínÇR</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address3</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/èZèä/ésí¨ë∫î‘ínÇR"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ìdòb</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">phone</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ìdòbî‘çÜ"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ågë—ìdòb</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">mobile</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ågë—ìdòb"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ìdéqÉÅÅ[Éã</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">hidden</xsl:attribute>
            <xsl:attribute name="NAME">email</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ìdéqÉÅÅ[Éã"/></xsl:attribute>
          </xsl:element>
          <xsl:text><xsl:value-of select="å⁄ãq/ìdéqÉÅÅ[Éã"/></xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>êEã∆</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">job</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/êEã∆"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ãŒñ±êÊ</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">company</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ãŒñ±êÊ"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ãªñ°ÇP</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest1</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ãªñ°/ãªñ°ÇP"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ãªñ°ÇQ</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest2</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ãªñ°/ãªñ°ÇQ"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>ãªñ°ÇR</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest3</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="å⁄ãq/ãªñ°/ãªñ°ÇR"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
