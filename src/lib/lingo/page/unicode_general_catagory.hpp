#ifndef H_LINGO_PAGE_UNICODE_GENERAL_CATAGORY
#define H_LINGO_PAGE_UNICODE_GENERAL_CATAGORY

#include <lingo/platform/constexpr.hpp>

#include <cstdint>

namespace lingo
{
    namespace page
    {
        enum class unicode_general_catagory
        {
            none = 0,

            l = 0x0100,
            lu = l | 0x01,
            ll = l | 0x02,
            lt = l | 0x04,
            lm = l | 0x08,
            lo = l | 0x10,
            lc = lu | ll | lt,

            m = 0x0200,
            mn = m | 0x01,
            mc = m | 0x02,
            me = m | 0x04,

            n = 0x0400,
            nd = n | 0x01,
            nl = n | 0x02,
            no = n | 0x04,

            p = 0x0800,
            pc = p | 0x01,
            pd = p | 0x02,
            ps = p | 0x04,
            pe = p | 0x08,
            pi = p | 0x10,
            pf = p | 0x20,
            po = p | 0x40,

            s = 0x1000,
            sm = s | 0x01,
            sc = s | 0x02,
            sk = s | 0x04,
            so = s | 0x08,

            z = 0x2000,
            zs = z | 0x01,
            zl = z | 0x02,
            zp = z | 0x04,

            c = 0x4000,
            cc = c | 0x01,
            cf = c | 0x02,
            cs = c | 0x04,
            co = c | 0x08,
            cn = c | 0x10,

            letter = l,
            letter_uppercase = lu,
            letter_lowercase = ll,
            letter_titlecase = lt,
            letter_modifier = lm,
            letter_other = lo,
            letter_cased = lc,

            mark = m,
            mark_nonspacing = mn,
            mark_spacing_combining = mc,
            mark_enclosing = me,

            punctuation = p,
            punctuation_connector = pc,
            punctuation_dash = pd,
            punctuation_open = po,
            punctuation_close = pc,
            punctuation_initual_quote = pi,
            punctuation_final_quote = pf,
            punctuation_other = po,

            symbol = s,
            symbol_math = sm,
            symbol_currency = sc,
            symbol_modifier = sk,
            symbol_other = so,

            separator = z,
            separator_space = zs,
            separator_line = zl,
            separator_paragraph = zp,

            other = c,
            other_control = cc,
            other_format = cf,
            other_surrogate = cs,
            other_private_use = co,
            other_unassigned = cs
        };

        LINGO_CONSTEXPR11 unicode_general_catagory major_catagory(unicode_general_catagory catagory)
        {
            return (unicode_general_catagory)(((std::uint_fast16_t)catagory) & 0xFF00);
        }
    }
}

#endif